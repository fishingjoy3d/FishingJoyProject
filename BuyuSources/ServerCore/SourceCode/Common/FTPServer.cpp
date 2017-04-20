#include "stdafx.h"
#include "FTPServer.h"
#define FTP_MIN_SIZE 65536
#define SEND_COMPLETION_COUNT 4
#define UDP_TEMP_BUFF_SIZE	2048
#define IP_REQUEST_TIMEOUT 500		//IP查询超时
#define MAX_PATH_LEN 700			//最大的路径长度
#define FILE_CHANGE_NUM 512			//文件改变通知的BUFF大小
#define TCP_SEND_BUFF_SIZE 128		//TCP发送大小
#define IP_HOST "opendata.baidu.com"
#define IP_REQUEST_STR "api.php?query=%d.%d.%d.%d&resource_id=6006&format=json"
#define XOR_MASK 0xAA
HashMap<UINT, UINT> g_IPCount;
static UINT WINAPI ThreadTrans(void *p)
{
	((FTPServer*)p)->_ThreadTrans();
	return 0;
}
static UINT WINAPI ThreadRecvCmd(void *p)
{
	((FTPServer*)p)->_ThreadRecvCmd();
	return 0;
}
static UINT WINAPI ThreadQueue(void *p)
{
	((FTPServer*)p)->_ThreadQueue();
	return 0;
}
static UINT WINAPI ThreadAccept(void *p)
{
	((FTPServer*)p)->_ThreadAccept();
	return 0;
}
static UINT WINAPI ThreadOpenFile(void *p)
{
	((FTPServer*)p)->_ThreadOpenFile();
	return 0;
}
static UINT WINAPI ThreadFileWatcher(void *p)
{
	((FTPServer*)p)->_ThreadFileWatcher();
	return 0;
}
void GetRequestStr(UINT ip, char str[256])
{
	sprintf_s(str, 256, IP_REQUEST_STR,
		(ip >> 0) & 0xff,
		(ip >> 8) & 0xff,
		(ip >> 16) & 0xff,
		(ip >> 24) & 0xff
		);
}
void ShowIPISP(UINT ip, BYTE isp, char str[256], uint Count)
{
	const char *pc = "电信";
	if (isp == ISP_LT)
		pc = "联通";
	else if (isp == ISP_YD)
		pc = "移动";
	sprintf_s(str, 256, "<IP:%d.%d.%d.%d(%s)，登录次数:%d>",
		(ip >> 0) & 0xff,
		(ip >> 8) & 0xff,
		(ip >> 16) & 0xff,
		(ip >> 24) & 0xff,
		pc,
		Count
		);
	Log(str);
}
UINT FTPServer::GetSendInterval(UINT sendErrCount, UINT baseInterval)
{
	UINT sendinterval;
	if (sendErrCount > 0)
	{
		if (sendErrCount >= m_InitData.SendSafeCount)
			sendinterval = m_InitData.Timeout;
		else
			sendinterval = m_InitData.SendErrorInterval;
		/*
		if (sendErrCount > SEND_SAFE_COUNT)
		sendinterval += SEND_ERROR_INTERVAL << (sendErrCount - SEND_SAFE_COUNT);
		else
		sendinterval += SEND_ERROR_INTERVAL;
		*/
		//Log("SendErrCount:%d, Interval:%d", sendErrCount, sendinterval);
	}
	else
		sendinterval = baseInterval;
	return sendinterval;
}
FTPServer::FTPServer() : m_FileModify(FILE_CHANGE_NUM)
{
	m_Socket = NULL;
	m_bRun = false;
}
FTPServer::~FTPServer()
{

}
bool FTPServer::Init(const FTPServerInitData &data)
{
	m_InitData = data;
	if (m_InitData.SocketSendBuffSize < FTP_MIN_SIZE)
		m_InitData.SocketSendBuffSize = FTP_MIN_SIZE;
	if ((m_InitData.PathLength = strlen(m_InitData.FTPPath)) == 0)
	{
		::GetCurrentDirectoryA(sizeof(m_InitData.FTPPath), m_InitData.FTPPath);
		m_InitData.PathLength = strlen(m_InitData.FTPPath);
	}
	for (UINT i = 0; i < m_InitData.PathLength; ++i)
	{
		if (m_InitData.FTPPath[i] == '\\')
			m_InitData.FTPPath[i] = '/';
	}
	if (m_InitData.FTPPath[m_InitData.PathLength - 1] != '/')
	{
		strcat_s(m_InitData.FTPPath, sizeof(m_InitData.FTPPath), "/");
		++m_InitData.PathLength;
	}
	Log("FTP Directory:%s", m_InitData.FTPPath);
	CreateSocketData csd;
	if (!CreateSocket(CST_TCP | CST_BIND, m_InitData.Port, 2048, 2048, csd))
		return false;

	m_Socket = csd.Socket;
	
	int ret = ::listen(m_Socket, 1024);
	if (ret != 0)
		return false;

	if (m_InitData.bFindISP)
	{
		HttpClientInitData hcd;
		hcd.SleepTime = 2;
		hcd.ThreadNum = 4;
		hcd.Timeout = 2000;
		m_HTTPClient.Init(hcd);
		m_HTTPClient.AddHost(IP_HOST);
	}
	m_pOpenFileThreadData = new FTPThreadData[m_InitData.RecvThreadNum];
	m_pRecvThreadData	= new FTPThreadData[m_InitData.RecvThreadNum];
	m_pQueueThreadData  = new FTPThreadData[m_InitData.RecvThreadNum];
	m_TransBandwidth	= UINT(m_InitData.Bandwidth * m_InitData.BandwidthUsePercent);
	m_TotalTransNum		= m_TransBandwidth / m_InitData.ClinetMinSpeed;
	m_TransNum			= 0;
	m_RecvCmdNum		= 0;
	m_SuccessNum		= 0;
	m_RecvThreadIdx		= 0;
	m_QueueNum			= 0;
	m_MaxQueueNum		= 0;
	m_MaxRecvCmdNum		= 0;
	m_MaxTransNum		= 0;
	m_TodayCompletionNum = 0;
	
	time_t t = time(NULL);
	localtime_s(&m_Today, &t);

	m_bRun = true;
	m_DirHandle			= CreateFileA(m_InitData.FTPPath,
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (m_DirHandle == INVALID_HANDLE_VALUE)
	{
		Log("监视目录失败:%s", m_InitData.FTPPath);
		return false;
	}

	Log("<%s, 总带宽:%d KB，传输带宽:%d KB，最大传输数量:%d，最小传输速度:%d KB，发送间隔:%d, 每帧最大发送字节:%d>", m_InitData.bFindISP? "查找IP" : "不查找IP", m_InitData.Bandwidth / 1024, m_TransBandwidth / 1024, m_TotalTransNum, m_InitData.ClinetMinSpeed / 1024, m_InitData.SendChunkInterval, m_InitData.PerMaxSendSize);
	for (UINT i = 0; i < m_InitData.RecvThreadNum; ++i)
		::_beginthreadex(0, 0, ThreadRecvCmd, this, 0, 0);
	::_beginthreadex(0, 0, ThreadTrans, this, 0, 0);
	::_beginthreadex(0, 0, ThreadQueue, this, 0, 0);
	::_beginthreadex(0, 0, ThreadOpenFile, this, 0, 0);
	::_beginthreadex(0, 0, ThreadFileWatcher, this, 0, 0);
	::_beginthreadex(0, 0, ThreadAccept, this, 0, 0);
	
	return true;
}
FTPFileData* FTPServer::GetFileHandle(FTPFileMap *pfilemap, char *pname, UINT nameLength)
{
	for (UINT i = 0; i < nameLength; ++i)
	{
		if (pname[i] == '\\')
			pname[i] = '/';
		else
			pname[i] = tolower(pname[i]);
	}
	pname[nameLength] = 0;
	UINT crc = AECrc32(pname, nameLength, 0);
	FTPFileData *pFileData = NULL;
	auto it = pfilemap->find(crc);
	if (it == pfilemap->end())
	{
		//打开新文件
		char filename[MAX_PATH_LEN];
		memcpy_s(filename, sizeof(filename), m_InitData.FTPPath, m_InitData.PathLength);
		memcpy_s(filename + m_InitData.PathLength, sizeof(filename), pname, nameLength);
		filename[m_InitData.PathLength + nameLength] = 0;
		FILE *pFile = NULL;
		fopen_s(&pFile, filename, "rb");
		//Log("打开文件:%s", pname);
		if (pFile != NULL)
		{
			fseek(pFile, 0, SEEK_END);
			UINT FileLength = ftell(pFile);
			UINT size = sizeof(FTPFileData)+FileLength;
			pFileData = (FTPFileData*)malloc(size);
			if (pFileData == NULL)
			{
				Log("文件读取失败，大小:%d", FileLength);
				fclose(pFile);
			}
			else
			{
				fseek(pFile, 0, SEEK_SET);
				int read = fread(pFileData->FileData, 1, FileLength, pFile);
				fclose(pFile);
				if (read != FileLength)
				{
					Log("文件读取长度不一致:%d, %d", read, FileLength);
					free(pFileData);
					pFileData = NULL;
				}
				pFileData->FileLength	= FileLength;
				pFileData->FileNameCrc	= crc;
				pFileData->UseCount		= 1;
				pFileData->LastTick		= 0;
				pfilemap->insert(make_pair(crc, pFileData));
			}
		}
		else
		{
			Log("<<<文件不存在:%s>>>", filename);
		}
	}
	else
	{
		//Log("打开已缓存文件:%s", pname);
		pFileData = it->second;
		pFileData->LastTick = 0;
		::InterlockedIncrement(&pFileData->UseCount);
	}
	return pFileData;
}
void	FTPServer::_ThreadFileWatcher()
{
	const int BUFF_SIZE = 8192;
	const int FILE_NAME = 500;
	DWORD cbBytes;
	char *pfile1	= new char[FILE_NAME];
	char *pfile2	= new char[FILE_NAME];
	char *pbuff		= new char[BUFF_SIZE];
	UINT flen		= 0;
	while (m_bRun)
	{
		memset(pbuff, 0, BUFF_SIZE);
		FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)pbuff;
		if (ReadDirectoryChangesW(m_DirHandle, pbuff, BUFF_SIZE, true,
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_CREATION |
			FILE_NOTIFY_CHANGE_SIZE,
			&cbBytes, NULL, NULL))
		{
			while (1)
			{
				if (pnotify->FileName)
					flen = WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, pfile1, FILE_NAME, NULL, NULL);
				for (UINT i = 0; i < flen; ++i)
				{
					if (pfile1[i] == '\\')
						pfile1[i] = '/';
					else
						pfile1[i] = tolower(pfile1[i]);
				}
				pfile1[flen] = 0;
				FTPFileModify ffm;
				ffm.FileNameCrc = AECrc32(pfile1, flen, 0);
				switch (pnotify->Action)
				{
				case FILE_ACTION_MODIFIED:
					//Log("{文件改变:%s}", pfile1);
					ffm.Opt = FTP_FILE_MODIFY;
					break;
				case FILE_ACTION_REMOVED:
					//Log("{文件删除:%s}", pfile1);
					ffm.Opt = FTP_FILE_DEL;
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					ffm.Opt = FTP_FILE_RENAME;
					if (pnotify->NextEntryOffset != 0 && (pnotify->FileNameLength > 0))
					{
						pnotify = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
						flen = WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, pfile2, FILE_NAME, NULL, NULL);
						for (UINT i = 0; i < flen; ++i)
						{
							if (pfile2[i] == '\\')
								pfile2[i] = '/';
							else
								pfile2[i] = tolower(pfile2[i]);
						}
						pfile2[flen] = 0;
						ffm.NewFileNameCrc = AECrc32(pfile2, flen, 0);
					}
					//Log("{文件重命名:%s : %s}", pfile1, pfile2);
					break;
				default:
					ffm.Opt = FTP_FILE_NONE;
					break;
				}
				if (ffm.Opt != FTP_FILE_NONE)
				{
					if (m_FileModify.HasSpace() == false)
					{
						Log("文件改变通知空间不足够!");
					}
					else
					{
						m_FileModify.AddItem(ffm);
					}
				}
				if (pnotify->NextEntryOffset != 0)
					pnotify = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
				else
					break;
			}
		}//end if
	}
}
FTPRecvState FTPServer::RecvData(FTPClientData *pc)
{
	if (pc->RecvSize < sizeof(NetCmd))
		return FTP_RECV_CONTINUE;
	NetCmd* pcmd = (NetCmd*)pc->Buff;
	if (pcmd->GetCmdSize() <= pc->RecvSize)
	{
		//接收到请求
		if (pcmd->GetCmdType() == CMD_CHECK_VERSION)
		{
			NetCmdCheckVersion *pcmdxml = (NetCmdCheckVersion*)pcmd;
			UINT cmdSize = (sizeof(NetCmdCheckVersion) + pcmdxml->Length) - 1;

			if (cmdSize != pcmd->GetCmdSize() || cmdSize - sizeof(NetCmdCheckVersion) > MAX_PATH_LEN)
			{
				Log("CheckVersion文件长度与命令不匹配,IP:%s, Length&CmdSize:%d, CmdSize:%d", pc->IPStr, cmdSize, pcmd->GetCmdSize());
				return FTP_RECV_ERROR;
			}
			pcmdxml->Mac[sizeof(pcmdxml->Mac) - 1] = 0;
			pc->CheckVer = true;
			pc->RequestFileCount   = 1;
			pc->RequestFileList[0].pcName = pcmdxml->FileName;
			pc->RequestFileList[0].NameLength = pcmdxml->Length;
			pc->MacCrc = AECrc32(&pc->IP, 4, 0);
			pc->MacCrc = AECrc32(pcmdxml->Mac, strlen(pcmdxml->Mac), pc->MacCrc);
			pcmdxml->FileName[pcmdxml->Length] = 0;
			return FTP_RECV_OK;
		}
		else if (pcmd->GetCmdType() == CMD_FILE_REQUEST)
		{
			NetCmdFileRequest *pcmdfile = (NetCmdFileRequest*)pcmd;
			UINT cmdSize = (sizeof(NetCmdFileRequest) + pcmdfile->Length - 1);
			if (cmdSize != pcmd->GetCmdSize() || cmdSize - sizeof(NetCmdFileRequest) > MAX_PATH_LEN)
			{
				Log("FileRequest文件长度与命令不匹配,IP:%s, Length&CmdSize:%d, CmdSize:%d", pc->IPStr, cmdSize, pcmd->GetCmdSize());
				return FTP_RECV_ERROR;
			}
			//分析字符串
			char *pend = pcmdfile->FileName + pcmdfile->Length;
			char *pstart = pcmdfile->FileName;
			BYTE fileidx = 0;
			while (pstart + 4 < pend)
			{
				USHORT namelen = strlen(pstart);
				if (namelen >= pend - pstart || namelen == 0 || fileidx >= MAX_FILE_COUNT)
					return FTP_RECV_ERROR;
				
				pc->RequestFileList[fileidx].pcName		= pstart;
				pc->RequestFileList[fileidx].Offset		= 0;
				pc->RequestFileList[fileidx].Length		= 0;
				pc->RequestFileList[fileidx].NameLength = namelen;
				pstart += namelen + 1;
				++fileidx;
			}
			pc->RequestFileCount = fileidx;
			return FTP_RECV_OK;
		}
		else if (pcmd->GetCmdType() == CMD_FILE_REQUEST_EX)
		{
			NetCmdFileRequest *pcmdfile = (NetCmdFileRequest*)pcmd;
			UINT cmdSize = (sizeof(NetCmdFileRequest)+pcmdfile->Length - 1);
			if (cmdSize != pcmd->GetCmdSize() || cmdSize - sizeof(NetCmdFileRequest) > MAX_PATH_LEN)
			{
				Log("FileRequest文件长度与命令不匹配,IP:%s, Length&CmdSize:%d, CmdSize:%d", pc->IPStr, cmdSize, pcmd->GetCmdSize());
				return FTP_RECV_ERROR;
			}
			//分析字符串
			char *pend = pcmdfile->FileName + pcmdfile->Length;
			char *pstart = pcmdfile->FileName;
			BYTE fileidx = 0;
			while (pstart + 4 < pend)
			{
				UINT offset = *((UINT*)pstart);
				pstart += 4;
				UINT length = *((UINT*)pstart);
				pstart += 4;
				USHORT namelen = strlen(pstart);
				if (namelen >= pend - pstart || namelen == 0 || fileidx >= MAX_FILE_COUNT)
					return FTP_RECV_ERROR;

				pc->RequestFileList[fileidx].pcName = pstart;
				pc->RequestFileList[fileidx].Offset = offset;
				pc->RequestFileList[fileidx].Length = length;
				pc->RequestFileList[fileidx].NameLength = namelen;
				pstart += namelen + 1;
				++fileidx;
			}
			pc->XorData = (pcmdfile->FileCount & 0x8000) != 0;
			pc->RequestFileCount = fileidx;
			return FTP_RECV_OK;
		}
		else
		{
			Log("未知的请求:%s, CmdType:%d, CmdSize:%d", pc->IPStr, pcmd->GetCmdType(), pcmd->GetCmdSize());
			return FTP_RECV_ERROR;
		}
	}
	else if (pcmd->GetCmdSize() > m_InitData.BuffSize)
	{
		Log("<命令大小超出限制:%s, 大小:%d>", pc->IPStr, pcmd->GetCmdSize());
		return FTP_RECV_ERROR;
	}
	return FTP_RECV_CONTINUE;
}
void FTPServer::CheckFileList(FTPFileMap *pFileMap, FTPFileList *pFileList)
{
	UINT tick = timeGetTime();
	while (m_FileModify.HasItem())
	{
		FTPFileModify ffm = m_FileModify.GetItem();
		auto it = pFileMap->find(ffm.FileNameCrc);
		if (it != pFileMap->end())
		{
			if (ffm.Opt == FTP_FILE_RENAME)
			{
				FTPFileData *pdata = it->second;
				pFileMap->erase(it);
				pdata->FileNameCrc = ffm.NewFileNameCrc;
				pFileMap->insert(make_pair(pdata->FileNameCrc, pdata));
				Log("<收到文件改名通知:%u : %u>", ffm.FileNameCrc, ffm.NewFileNameCrc);
			}
			else if (ffm.Opt == FTP_FILE_DEL || ffm.Opt == FTP_FILE_MODIFY)
			{
				pFileList->push_back(it->second);
				pFileMap->erase(it);
				Log("<收到文件移除/改变通知:%u>", ffm.FileNameCrc);
			}
		}
	}
	for (UINT i = 0; i < pFileList->size();)
	{
		FTPFileData *pfile = (*pFileList)[i];
		if (pfile->UseCount == 0)
		{
			if (pfile->LastTick == 0)
			{
				pfile->LastTick = tick;
			}
			else if (tick - pfile->LastTick > m_InitData.FileCacheTime)
			{
				Log("清理移除的缓存文件:%u, Tick:%d", pfile->FileNameCrc, tick - pfile->LastTick);
				free(pfile);
				ListRemoveAt(*pFileList, i);
				continue;
			}
		}
		++i;
	}
	for (auto it = pFileMap->begin(); it != pFileMap->end();)
	{
		FTPFileData *pfile = it->second;
		if (pfile->UseCount == 0)
		{
			if (pfile->LastTick == 0)
			{
				pfile->LastTick = tick;
			}
			else if (tick - pfile->LastTick > m_InitData.FileCacheTime)
			{
				Log("清理缓存文件:%u, Tick:%d", pfile->FileNameCrc, tick - pfile->LastTick);
				free(pfile);
				it = pFileMap->erase(it);
				continue;
			}
		}
		++it;
	}
}
uint FTPServer::InitFileData(FTPClientData *pc)
{
	//1.计算文件的起始ID。
	//2.计算文件的最大ID
	//3.计算文件是否发送完成。
	uint idx = pc->RequestFileCount;
	for (UINT i = 0; i < pc->RequestFileCount; ++i)
	{
		FileNameData &fnd = pc->RequestFileList[i];
		FTPFileData *fileData = pc->pFileHandler[i];
		if (fnd.Offset < fileData->FileLength && idx > i)
			idx = i;
	}
	//if (idx != 0)
	return idx;
}
void XorData(BYTE *pdata, UINT size)
{
	//还原数据
	BYTE *pend = pdata + size;
	while (pdata != pend)
	{
		*pdata = *pdata ^ XOR_MASK;
		++pdata;
	}
}
void FTPServer::_ThreadTrans()
{
	vector<FTPClientData*> sizeList;
	vector<FTPClientData*> chunkList;
	timeval timev = { 0, 0 };
	FD_SET *pSet = CreateFDSet();
	const UINT SEC_BAND_WIDTH = m_TransBandwidth / (1000 / m_InitData.SendChunkInterval);
	int		ret = 0;
	UINT	perSpeed = 0;
	UINT	tick = 0;
	UINT	transIdx = 0;
	UINT	checkDayTick = timeGetTime();
	char	buff[256];
	UINT	*pUDPSendBuff = (UINT*)malloc(UDP_TEMP_BUFF_SIZE);
	UINT	CompletionMask = 0xffffffff;
	while (m_bRun)
	{
		tick = timeGetTime();
		if (tick - checkDayTick > 10000)
		{
			checkDayTick = tick;
			struct tm curday;
			time_t t = ::time(NULL);
			localtime_s(&curday, &t);
			if (curday.tm_year != m_Today.tm_year ||
				curday.tm_mon != m_Today.tm_mon ||
				curday.tm_mday != m_Today.tm_mday)
			{
				m_TodayCompletionNum = 0;
				m_Today = curday;
			}
		}
		//1.新加入的客户端
		//=========================================
		while (m_TransThreadData.NewClients.HasItem())
		{
			FTPClientData *pc = m_TransThreadData.NewClients.GetItem();
			pc->SendErrorCount = 0;
			pc->Tick = tick;
			pc->OnlineTick = tick;
			sizeList.push_back(pc);
		}
		tick = timeGetTime();
		//发送文件大小和个数
		for (UINT i = 0; i < sizeList.size();)
		{
			FTPClientData *pc = sizeList[i];
			if (tick - pc->Tick > m_InitData.Timeout)
			{
				//Log("发送文件大小超时:%s, 已发送:%d, 时长:%d", pc->IPStr, pc->SendSize, tick - pc->OnlineTick);
				RemoveClient(pc, FTP_REMOVE_TRANS_TIMEOUT);
				ListRemoveAt(sizeList, i);
				::InterlockedDecrement(&m_TransNum);
				continue;
			}
			UINT sendinterval = GetSendInterval(pc->SendErrorCount, m_InitData.SendChunkInterval);
			if (tick - pc->SendTick < sendinterval)
			{
				++i;
				continue;
			}
			pc->SendTick = tick;
			UINT *pSendData = (UINT*)(buff);
			*pSendData = pc->RequestFileCount | 0xC0000000;
			if (m_InitData.bFindISP)
			{
				*pSendData |= uint(pc->ISPType) << 28;//只支持三种ISP，否则和文件大小冲突。变成0xF0000000。
			}
			++pSendData;
			for (int j = 0; j < pc->RequestFileCount; ++j)
			{
				uint sendSize = pc->RequestFileList[j].Length;
				*pSendData++ = sendSize | 0xF0000000;
			}

			UINT sendSize = (pc->RequestFileCount + 1) * sizeof(UINT);
			ret = send(pc->Socket, buff, sendSize, 0);
			if (ret == sendSize)
			{
				pc->SendFileIndex = InitFileData(pc);
				pc->SendSize = pc->RequestFileList[pc->SendFileIndex].Offset;
				pc->SendErrorCount = 0;
				pc->Tick = tick;
				ListRemoveAt(sizeList, i);
				chunkList.push_back(pc);
				continue;
			}
			else
			{
				UINT errCode = WSAGetLastError();
				if (errCode == WSAECONNRESET || errCode == WSAECONNABORTED)
				{
					RemoveClient(pc, FTP_REMOVE_TRANS_TIMEOUT);
					ListRemoveAt(sizeList, i);
					::InterlockedDecrement(&m_TransNum);
					continue;
				}
				else
				{
					++pc->SendErrorCount;
				}
				++i;
			}
		}
		
		//2.传输数据
		//=========================================
		if (chunkList.size() == 0)
			goto SLEEP;
		tick = timeGetTime();
		//每个连接的最大传输
		perSpeed = min(m_InitData.PerMaxSendSize, SEC_BAND_WIDTH / chunkList.size());
		FD_ZERO(pSet);
		for (auto it = chunkList.begin(); it != chunkList.end(); ++it)
		{
			FD_ADD((*it)->Socket, pSet);
		}
		select(0, 0, pSet, 0, &timev);
		for (UINT i = 0; i < chunkList.size();)
		{
			FTPClientData *pc = chunkList[i];
			if (tick - pc->Tick > m_InitData.Timeout)
			{
				Log("发送数据超时(%u):%s, 时长:%d, 发送:%d, SendErrCount:%d, Port:%d", tick - pc->Tick, pc->IPStr, tick - pc->OnlineTick, pc->TotalSendSize, pc->SendErrorCount, pc->Port);
				RemoveClient(pc, FTP_REMOVE_TRANS_TIMEOUT);
				ListRemoveAt(chunkList, i);
				::InterlockedDecrement(&m_TransNum);
				continue;
			}
			if (tick - pc->SendTick < m_InitData.SendChunkInterval || FD_ISSET(pc->Socket, pSet) == false)
			{
				++i;
				continue;
			}
			FTPFileData *pFileData = pc->pFileHandler[pc->SendFileIndex];
			FileNameData &fnd = pc->RequestFileList[pc->SendFileIndex];
			UINT sendSize = (fnd.Offset + fnd.Length) - pc->SendSize;
			sendSize = min(perSpeed, sendSize);
			//异或数据，防止发送失败
			if (pc->XorData)
			{
				XorData((BYTE*)pFileData->FileData + pc->SendSize, sendSize);
			}
			ret = send(pc->Socket, pFileData->FileData + pc->SendSize, sendSize, 0);
			//还原数据
			if (pc->XorData)
				XorData((BYTE*)pFileData->FileData + pc->SendSize, sendSize);
			if (ret == sendSize)
			{
				pc->Tick = tick;
				pc->SendTick = tick;
				pc->SendErrorCount = 0;
				pc->SendSize += sendSize;
				pc->TotalSendSize += sendSize;
				if (pc->SendSize == (fnd.Offset + fnd.Length))
				{
					//Log("发送文件完成:%d, Size:%d", pc->SendFileIndex, pFileData->FileLength);
					while (++pc->SendFileIndex < pc->RequestFileCount)
					{
						if (pc->RequestFileList[pc->SendFileIndex].Offset < pc->pFileHandler[pc->SendFileIndex]->FileLength)
							break;
					}
					if (pc->SendFileIndex < pc->RequestFileCount)
					{
						pc->SendSize = pc->RequestFileList[pc->SendFileIndex].Offset;
					}
					else
					{
						//float time = (tick - pc->OnlineTick) / 1000.0f;
						//float spd = (pc->TotalSendSize / 1024.0f) / time;
						//Log("发送完成:%s, 大小:%0.2f KB, 耗时:%0.2f 秒, 速度:%0.2f KB", pc->IPStr, pc->TotalSendSize / 1024.0f, time, spd);
						if (pc->CheckVer)
						{
							UINT count = 1;
							auto it = g_IPCount.find(pc->MacCrc);
							if (it == g_IPCount.end())
							{
								g_IPCount.insert(make_pair(pc->MacCrc, 1));
								++m_SuccessNum;
								++m_TodayCompletionNum;
							}
							else
							{
								count = ++it->second;
							}
							ShowIPISP(pc->IP, pc->ISPType, buff, count);
						}
						RemoveClient(pc, FTP_REMOVE_TRANS_COMPLETION);
						::InterlockedDecrement(&m_TransNum);
						ListRemoveAt(chunkList, i);
						continue;
					}
				}
			}
			else
			{
				UINT errCode = WSAGetLastError();
				if (errCode == WSAECONNRESET || errCode == WSAECONNABORTED)
				{
					//断开
					Log("发送数据断开:%s, 已发送:%d/%d, 时长:%d", pc->IPStr, pc->SendSize, pc->TotalSendSize, tick - pc->OnlineTick);
					RemoveClient(pc, FTP_REMOVE_TRANS_TIMEOUT);
					ListRemoveAt(chunkList, i);
					::InterlockedDecrement(&m_TransNum);
					continue;
				}
				else
				{
					Log("发送数据等待, ErrCode:%d, Count:%d, Tick:%d, Send:%d", WSAGetLastError(), pc->SendErrorCount + 1, tick - pc->Tick, sendSize);
					++pc->SendErrorCount;
				}
			}
			++i;
		}//end chunkList.
	SLEEP:
		Sleep(m_InitData.SleepTime);
	}
}

void FTPServer::_ThreadOpenFile()
{
	vector<FTPClientData*> clientList;
	vector<FTPClientData*> ipList;
	HashMap<UINT, BYTE> ipinfoList;
	char tmpstr[256];
	HttpClientResult *hcr;
	while (m_bRun)
	{
		CheckFileList(&m_FileList, &m_FreeFileList);

		for (UINT i = 0; i < m_InitData.RecvThreadNum; ++i)
		{
			while (m_pOpenFileThreadData[i].NewClients.HasItem())
			{
				FTPClientData *pc = m_pOpenFileThreadData[i].NewClients.GetItem();
				clientList.push_back(pc);
			}
		}
		UINT tick = timeGetTime();
		for (UINT n = 0; n < clientList.size();)
		{
			FTPClientData *pc = clientList[n];
			for (USHORT i = 0; i < pc->RequestFileCount; ++i)
			{
				FileNameData &recvFile = pc->RequestFileList[i];
				FTPFileData *pfile = GetFileHandle(&m_FileList, recvFile.pcName, recvFile.NameLength);
				if (pfile != null)
				{
					for (int j = 0; j < i; ++j)
					{
						if (pc->pFileHandler[j] == pfile)
						{
							//重复请求一个文件。
							pc->OpenFileState = FTP_OPEN_FAILED;
							goto OPEN_CHECK_END;
						}
					}
					if (recvFile.Offset == pfile->FileLength)
					{

					}
					else if (recvFile.Offset + recvFile.Length > pfile->FileLength)
					{
						Log("断点偏移大于文件的大小:%d vs %d", recvFile.Offset + recvFile.Length, pfile->FileLength);
						pc->OpenFileState = FTP_OPEN_FAILED;
						goto OPEN_CHECK_END;
					}
					if (recvFile.Length == 0)
						recvFile.Length = pfile->FileLength - recvFile.Offset;
					pc->pFileHandler[i] = pfile;
				}
				else
				{
					pc->OpenFileState = FTP_OPEN_FAILED;
					goto OPEN_CHECK_END;
				}//end if(pfile != null)
			
			}//end for 文件循环

			if (m_InitData.bFindISP == false)
			{
				pc->OpenFileState = FTP_OPEN_OK;
			}
			else
			{
				//IP属于什么类型
				auto it = ipinfoList.find(pc->IP);
				if (it != ipinfoList.end())
				{
					pc->ISPType = it->second;
					//ShowIPISP(pc->IP, pc->ISPType, tmpstr);
					pc->OpenFileState = FTP_OPEN_OK;

				}
				else
				{
					//等待IP查询
					pc->IPRequestTick = tick;
					GetRequestStr(pc->IP, tmpstr);
					m_HTTPClient.AddRequest((UINT_PTR)pc->IP, 0, tmpstr);
					ipList.push_back(pc);
				}
			}
		OPEN_CHECK_END:
			ListRemoveAt(clientList, n);
		}

		//IP获取
		//=======================================
		if (m_InitData.bFindISP)
		while (m_HTTPClient.GetResult(hcr))
		{
			if (hcr->State == REQUEST_OK)
			{
				BYTE ispType = ISP_DX;;
				char *pfind = strstr(hcr->RecvBuff, "\"location\"");
				if (pfind != NULL)
				{
					char *pend = hcr->RecvBuff + hcr->RecvSize;
					while (pfind < pend)
					{
						if (*pfind == ' ')
						{
							++pfind;
							if (strncmp(pfind, "电信", 4) == 0)
								ispType = ISP_DX;
							else if (strncmp(pfind, "联通", 4) == 0)
								ispType = ISP_LT;
							else if (strncmp(pfind, "移动", 4) == 0)
								ispType = ISP_YD;
							break;
						}
						else
							++pfind;
					}
				}
				ipinfoList.insert(make_pair(hcr->ID, ispType));
			}
			m_HTTPClient.FreeResult(hcr);
		}
		//查询IP结果
		//=======================================
		tick = timeGetTime();
		for (UINT i = 0; i < ipList.size(); )
		{
			FTPClientData *pc = ipList[i];
			auto it = ipinfoList.find(pc->IP);
			if (it != ipinfoList.end())
			{
				pc->ISPType = it->second;
				pc->OpenFileState = FTP_OPEN_OK;
				//ShowIPISP(pc->IP, pc->ISPType, tmpstr);
			}
			else if (tick - pc->IPRequestTick > IP_REQUEST_TIMEOUT)
			{
				pc->ISPType = ISP_DX;
				pc->OpenFileState = FTP_OPEN_OK;
			}
			else
			{
				++i;
				continue;
			}
			ListRemoveAt(ipList, i);
		}
		Sleep(m_InitData.SleepTime);
	}
}
void FTPServer::_ThreadRecvCmd()
{
	UINT idx = ::InterlockedIncrement(&m_RecvThreadIdx) - 1;
	vector<FTPClientData*>	recvCmdList;
	vector<FTPClientData*>	openFileList;

	FD_SET	*pSet = CreateFDSet();
	timeval time = { 0, 0 };
	int		ret = 0;
	uint	openFileState = OPEN_FILE_STATE;
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		while (m_pRecvThreadData[idx].NewClients.HasItem())
		{
			FTPClientData *pc = m_pRecvThreadData[idx].NewClients.GetItem();
			pc->Tick = tick;
			pc->SendErrorCount = 0;
			recvCmdList.push_back(pc);
		}

		FD_ZERO(pSet);
		for (UINT i = 0; i < recvCmdList.size();)
		{
			FTPClientData *pc = recvCmdList[i];
			if (tick - pc->Tick > m_InitData.Timeout)
			{
				//Log("接收命令超时:%s, 时长:%u", pc->IPStr, tick - pc->OnlineTick);
				::InterlockedDecrement(&m_RecvCmdNum);
				RemoveClient(pc, FTP_REMOVE_RECVCMD_TIMEOUT);
				ListRemoveAt(recvCmdList, i);
				continue;
			}
			FD_ADD(pc->Socket, pSet);
			++i;
		}
		if (FD_COUNT(pSet) == 0)
			goto OPEN_FILE;

		select(0, pSet, 0, 0, &time);
		tick = timeGetTime();
		for (UINT i = 0; i < recvCmdList.size();)
		{
			FTPClientData *pc = recvCmdList[i];
			if (FD_ISSET(pc->Socket, pSet) == false)
			{
				++i;
				continue;
			}
			ret = recv(pc->Socket, pc->Buff, m_InitData.BuffSize - pc->RecvSize, 0);
			if (ret == 0 || (ret == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAECONNABORTED)))
			{
				//Log("接收命令断开:%s，时长:%u", pc->IPStr, tick - pc->OnlineTick);
				::InterlockedDecrement(&m_RecvCmdNum);
				RemoveClient(pc, FTP_REMOVE_RECVCMD_TIMEOUT);
				ListRemoveAt(recvCmdList, i);
				continue;
			}
			if (ret > 0)
			{
				pc->RecvSize += ret;
				pc->Tick = tick;
				FTPRecvState rs = RecvData(pc);
				switch (rs)
				{
				case FTP_RECV_OK:
					pc->OpenFileState = FTP_OPEN_WAIT;
					ListRemoveAt(recvCmdList, i);
					openFileList.push_back(pc);
					m_pOpenFileThreadData[idx].NewClients.AddItem(pc);
					continue;
				case FTP_RECV_ERROR:
				case FTP_RECV_FILE_ERROR:
					::InterlockedDecrement(&m_RecvCmdNum);
					RemoveClient(pc, FTP_REMOVE_RECVCMD_ERROR);
					ListRemoveAt(recvCmdList, i);
					continue;
				}
			}
			++i;
		}

	OPEN_FILE:
		tick = timeGetTime();
		for (UINT i = 0; i < openFileList.size(); )
		{
			FTPClientData *pc = openFileList[i];
			if (tick - pc->Tick > m_InitData.Timeout)
			{
				//超时，等待返回结果然后删除。
				if (pc->OpenFileState == FTP_OPEN_OK || pc->OpenFileState == FTP_OPEN_FAILED)
				{
					::InterlockedDecrement(&m_RecvCmdNum);
					RemoveClient(pc, FTP_REMOVE_OPEN_TIMEOUT);
					ListRemoveAt(openFileList, i);
				}
				else
				{
					++i;
				}
				continue;
			}
			else if (pc->OpenFileState == FTP_OPEN_OK)
			{
				//加载文件成功
				::InterlockedDecrement(&m_RecvCmdNum);
				m_pQueueThreadData[idx].NewClients.AddItem(pc);
				ListRemoveAt(openFileList, i);
			}
			else if (pc->OpenFileState == FTP_OPEN_FAILED)
			{
				//加载文件失败.
				::InterlockedDecrement(&m_RecvCmdNum);
				RemoveClient(pc, FTP_REMOVE_OPEN_FAILED);
				ListRemoveAt(openFileList, i);
			}
			else
			{
				UINT interval = GetSendInterval(pc->SendErrorCount, m_InitData.SendQueueInterval);
				if (tick - pc->SendTick > interval)
				{
					pc->SendTick = tick;
					if (4 == send(pc->Socket, (char*)&openFileState, 4, 0))
					{
						pc->SendErrorCount = 0;
						pc->Tick = tick;
					}
					else
					{
						UINT errCode = WSAGetLastError();
						if (errCode == WSAECONNRESET || errCode == WSAECONNABORTED)
						{
							//已经断开，设置超时
							pc->Tick = 0;
							pc->SendErrorCount = m_InitData.SendSafeCount;
							continue;
						}
						else
						{
							++pc->SendErrorCount;
						}
					}
				}
				++i;
			}
		}

		Sleep(m_InitData.SleepTime);
	}
}
void FTPServer::_ThreadQueue()
{
	list<FTPClientData*>	queueList;
	int		ret		= 0;
	while (m_bRun)
	{
		UINT tick = timeGetTime();
		for (UINT i = 0; i < m_InitData.RecvThreadNum; ++i)
		{
			while (m_pQueueThreadData[i].NewClients.HasItem())
			{
				FTPClientData *pc = m_pQueueThreadData[i].NewClients.GetItem();
				pc->Tick = tick;
				pc->SendErrorCount = 0;
				++m_QueueNum;
				if (m_QueueNum > m_MaxQueueNum)
					m_MaxQueueNum = m_QueueNum;
				queueList.push_back(pc);
			}
		}
		UINT queueIdx = 1;
		UINT transNum = m_TransNum;
		for (auto it = queueList.begin(); it != queueList.end();)
		{
			FTPClientData *pc = *it;
			if (tick - pc->Tick > m_InitData.Timeout)
			{
				//Log("排队超时:%s, 时长:%u", pc->IPStr, tick - pc->OnlineTick);
				RemoveClient(pc, FTP_REMOVE_QUEUE_TIMEOUT);
				it = queueList.erase(it);
				--m_QueueNum;
				continue;
			}
			if (transNum < m_TotalTransNum)
			{
				//添加到发送
				if (m_TransThreadData.NewClients.HasSpace() == false)
				{
					Log("传输线程没有足够的空间!");
				}
				else
				{
					--m_QueueNum;
					++transNum;
					m_TransThreadData.NewClients.AddItem(pc);
					it = queueList.erase(it);
					UINT tn = ::InterlockedIncrement(&m_TransNum);
					if (tn > m_MaxTransNum)
						m_MaxTransNum = tn;
					continue;
				}
			}
			++queueIdx;
			UINT interval = GetSendInterval(pc->SendErrorCount, m_InitData.SendQueueInterval);
			if (tick - pc->SendTick < interval)
			{
				++it;
				continue;
			}
			//发送排队的顺序
			pc->SendTick = tick;
			UINT data = queueIdx | 0x80000000;
			ret = send(pc->Socket, (char*)&data, 4, 0);
			if (ret == 4)
			{
				pc->Tick = tick;
				pc->SendErrorCount = 0;
				++it;
			}
			else
			{
				UINT errCode = WSAGetLastError();
				if (errCode == WSAECONNRESET || errCode == WSAECONNABORTED)
				{
					--m_QueueNum;
					//Log("发送排队断开:%s, 时长:%u", pc->IPStr, tick - pc->OnlineTick);
					//已经断开
					RemoveClient(pc, FTP_REMOVE_QUEUE_TIMEOUT);
					it = queueList.erase(it);
				}
				else
				{
					++pc->SendErrorCount;
					++it;
				}
			}
		}
		Sleep(m_InitData.SleepTime);
	}
}
void FTPServer::_ThreadAccept()
{
	SOCKADDR_IN addr;
	SOCKADDR_IN addr2;
	int addrSize = sizeof(addr);
	ULONG arg = 0;
	UINT tick = 0;
	UINT maxframetick = 0;
	UINT addIdx = 0;
	UINT newAdd = 0;

	while (m_bRun)
	{
		for (UINT i = 0; i < m_InitData.AcceptPerFrame; ++i)
		{
			SOCKET s = accept(m_Socket, (sockaddr*)&addr, &addrSize);
			if (s == INVALID_SOCKET)
				break;
			
			InitSocket(s, m_InitData.SocketSendBuffSize, m_InitData.SocketRecvBuffSize, true);
			getsockname(s, (struct sockaddr*)&addr2, &addrSize);
			FTPClientData *cd	= new(malloc(sizeof(FTPClientData)+m_InitData.BuffSize)) FTPClientData;
			memset(cd, 0, sizeof(FTPClientData));
			cd->Socket			= s;
			cd->IP				= addr.sin_addr.S_un.S_addr;
			cd->LocalIP			= addr2.sin_addr.S_un.S_addr;
			cd->Port			= ntohs(addr.sin_port);
			cd->Tick			= cd->OnlineTick = timeGetTime();
			cd->IPStr[0]		= 0;
			//GetIPString(cd->IP, cd->Port, cd->IPStr);
			UINT idx = ++addIdx % m_InitData.RecvThreadNum;
			if (m_pRecvThreadData[idx].NewClients.HasSpace() == false)
			{
				RemoveClient(cd, FTP_REMOVE_THREAD_NOTSPACE);
				Log("Recv线程没有足够的空间.");
			}
			else
			{
				UINT recvNum = ::InterlockedIncrement(&m_RecvCmdNum);
				++newAdd;
				m_pRecvThreadData[idx].NewClients.AddItem(cd);
			}
		}
		if (timeGetTime() - tick > 10000)
		{
			tick = timeGetTime();
			Log("连接:%d/%d, 排队:%d/%d, 下载:%d/%d, 完成:%d/%d", m_RecvCmdNum, newAdd, m_QueueNum, m_MaxQueueNum, m_TransNum, m_MaxTransNum, m_TodayCompletionNum, m_SuccessNum);
		}
		Sleep(m_InitData.SleepTime);
	}
}
void FTPServer::RemoveClient(FTPClientData *pclient, FTPRemoveType remove)
{
	for (int i = 0; i < pclient->RequestFileCount; ++i)
	{
		if (pclient->pFileHandler[i])
			::InterlockedDecrement(&pclient->pFileHandler[i]->UseCount);
	}
	if (pclient->pResendList)
		delete pclient->pResendList;
	closesocket(pclient->Socket);
	pclient->~FTPClientData();
	free(pclient);
}