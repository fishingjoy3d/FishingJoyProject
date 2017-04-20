#include "Stdafx.h"
#include "RankManager.h"
#include "FishServer.h"
#define RankFilePath "xml\\"
RankManager::RankManager()
{
	m_LoadSum = 0;
	m_RankArray.clear();
}
RankManager::~RankManager()
{
}
void RankManager::OnInit(bool IsNeedLoadNowDayRank)
{
	if (IsNeedLoadNowDayRank)
	{
		OnLoadRankInfoByDB();//是否需要重新生成配置文件
	}
}
void RankManager::OnDayChange()
{
	//我们应该提前生成排行榜文件 在头一天的23:50 分钟生成文件 提前生成排行榜文件 并且上传到FTP服务器 23:50
	//23:50到明天的23:50  属于明天 23:50之前属于今天
	OnLoadRankInfoByDB();//发送命令加载当天的数据 排行榜数据
}
void RankManager::OnLoadRankInfoByDB()
{
	m_LoadSum = 0;
	m_RankArray.clear();
	HashMap<BYTE, tagRankConfig>::iterator Iter = g_FishServer.GetFishConfig().GetRankConfig().m_RankMap.begin();
	for (; Iter != g_FishServer.GetFishConfig().GetRankConfig().m_RankMap.end(); ++Iter)
	{
		DBR_Cmd_LoadRankInfo msg;
		SetMsgInfo(msg,DBR_LoadRankInfo, sizeof(DBR_Cmd_LoadRankInfo));
		msg.RankID = Iter->second.RankID;
		msg.RandRowCount = Iter->second.RowCount;
		g_FishServer.SendNetCmdToDB(&msg);
	}
}
void RankManager::OnLoadRankInfoResult(DBO_Cmd_LoadRankInfo* pDB)
{
	//返回加载的结果 我们将当天的数据保存在集合里面去
	if (!pDB)
	{
		ASSERT(false);
		return;
	}
	if (pDB->Sum > 0)
	{
		HashMap<BYTE, std::vector<tagRankInfo>>::iterator Iter = m_RankArray.find(pDB->RankID);
		if (Iter == m_RankArray.end())
		{
			std::vector<tagRankInfo> pVec;
			for (int i = 0; i < pDB->Sum; ++i)
			{
				pVec.push_back(pDB->Array[i]);
			}
			m_RankArray.insert(std::map<BYTE, std::vector<tagRankInfo>>::value_type(pDB->RankID, pVec));
		}
		else
		{
			for (int i = 0; i < pDB->Sum; ++i)
			{
				Iter->second.push_back(pDB->Array[i]);
			}
		}
	}
	if ((pDB->States & MsgEnd) != 0)
	{
		++m_LoadSum;
	}
	if (ConvertIntToDWORD(m_LoadSum) == g_FishServer.GetFishConfig().GetRankConfig().m_RankMap.size())
	{
		//数据已经读取完毕了 我们可以生成文件了
		SaveNowDayRankInfo();
	}
}
void RankManager::SaveNowDayRankInfo()
{
	//将文件进行保存
	if (m_RankArray.empty())
		return;
	//空白的无需写配置文件  3:50  我们应该保存今天的文件  有可能是中间的时间
	DWORD AllRankSum = 0;

	time_t tm = time(NULL) + Diff_Update_Sec - g_FishServer.GetFishConfig().GetWriteSec();
	SYSTEMTIME time = TimeTToSystemTime(tm);

	char FileName[32] = { 0 };
	sprintf_s(FileName, CountArray(FileName), "Rank_%d_%d_%d.xml", time.wYear, time.wMonth, time.wDay);
	tinyxml2::XMLDocument xmlDoc;
	XMLDeclaration* Declaration = xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"");
	if (!Declaration)
	{
		ASSERT(false);
		return;
	}
	xmlDoc.InsertEndChild(Declaration);
	tinyxml2::XMLElement* elmRank = xmlDoc.NewElement("FishRanks");
	if (!elmRank)
	{
		ASSERT(false);
		return;
	}
	HashMap<BYTE, std::vector<tagRankInfo>>::iterator Iter = m_RankArray.begin();
	for (; Iter != m_RankArray.end(); ++Iter)
	{
		if (Iter->second.empty())
			continue;
		//1.开始写入节点
		tinyxml2::XMLElement* elmRook = xmlDoc.NewElement("FishRook");
		if (!elmRook)
			continue;
		elmRook->SetAttribute("TypeID", Iter->first);
		std::vector<tagRankInfo>::iterator IterVec = Iter->second.begin();
		for (int i = 1; IterVec != Iter->second.end(); ++IterVec, ++i)
		{
			//插入数据 
			tinyxml2::XMLElement* Info = xmlDoc.NewElement("Info");
			if (!Info)
				continue;
			Info->SetAttribute("Index", i);
			Info->SetAttribute("UserID", (unsigned int)IterVec->dwUserID);
			Info->SetAttribute("FaceID", (unsigned int)IterVec->dwFaceID);
			Info->SetAttribute("Gender", IterVec->Gender ? 1 : 0);
			Info->SetAttribute("TitleID", IterVec->TitleID);
			char Int64[64] = { 0 };
			errno_t Error = _i64toa_s(IterVec->Param, Int64, CountArray(Int64), 10);
			if (Error != 0)
			{
				ASSERT(false);
				continue;
			}
			Info->SetAttribute("Param", Int64);//Int64
			char * UName = UnicodeToutf8((char*)IterVec->NickName);
			Info->SetAttribute("Name", UName);
			free(UName);
			elmRook->InsertEndChild(Info);

			++AllRankSum;
		}
		elmRank->InsertEndChild(elmRook);
	}
	xmlDoc.InsertEndChild(elmRank);

	//保存到指定路径
	string FilePath = RankFilePath;
	FilePath  += FileName;
	xmlDoc.SaveFile(FilePath.c_str());
	SaveFileToFtp(FilePath.c_str(),FileName);//将文件传输到FTP去
	if (time.wDayOfWeek == g_FishServer.GetFishConfig().GetRankConfig().RankWeekDay)//创建一个星期排行榜的奖励文件
	{
		g_FishServer.ShowInfoToWin("生成新的排行榜文件 并且为当前周的排行榜: %s", FileName);

		DWORD VersionID = (time.wYear) * 10000 + time.wMonth * 100 + time.wDay;
		//将数据保存到数据库去 通过一个命令进行发送
		DWORD PageSize = sizeof(DBR_Cmd_SaveWeekRankInfo)+(AllRankSum - 1)*sizeof(tagRankWeekRankInfo);
		DBR_Cmd_SaveWeekRankInfo* msg = (DBR_Cmd_SaveWeekRankInfo*)malloc(PageSize);
		if (!msg)
		{
			ASSERT(false);
			return;
		}
		msg->SetCmdType(DBR_SaveWeekRankInfo);
		HashMap<BYTE, std::vector<tagRankInfo>>::iterator Iter = m_RankArray.begin();
		for (int Index =0; Iter != m_RankArray.end(); ++Iter)
		{
			if (Iter->second.empty())
				continue;
			std::vector<tagRankInfo>::iterator IterVec = Iter->second.begin();
			for (int i = 1; IterVec != Iter->second.end(); ++IterVec, ++i)
			{
				msg->Array[Index].dwUserID = IterVec->dwUserID;
				msg->Array[Index].IsReward = false;
				msg->Array[Index].RankID = Iter->first;
				msg->Array[Index].RankIndex = ConvertIntToBYTE(i);
				msg->Array[Index].VersionID = VersionID;
				++Index;
			}
		}
		std::vector<DBR_Cmd_SaveWeekRankInfo*> pVec;
		SqlitMsg(msg, PageSize, AllRankSum,false, pVec);
		free(msg);
		if (!pVec.empty())
		{
			std::vector<DBR_Cmd_SaveWeekRankInfo*>::iterator Iter = pVec.begin();
			for (; Iter != pVec.end(); ++Iter)
			{
				g_FishServer.SendNetCmdToDB(*Iter);
				free(*Iter);
			}
			pVec.clear();
		}
	}
	else
		g_FishServer.ShowInfoToWin("生成新的排行榜文件: %s", FileName);
	m_RankArray.clear();//当前的数据立刻清空掉
}
void RankManager::SaveFileToFtp(const char* FilePath,const char* FileName)
{
	//将指定文件生成到FTP服务器去 
	//1.加载指定文件到内存中来
	FILE* pFile = null;
	errno_t Error = fopen_s(&pFile, FilePath, "rb+");
	if (Error != 0 || !pFile)
		return;
	//获取文件的长度 
	fseek(pFile, 0, SEEK_END);
	int FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	DWORD PageSize = sizeof(CF_Cmd_UpLoadFile)+(FileSize - 1)* sizeof(BYTE);
	CF_Cmd_UpLoadFile* msg = (CF_Cmd_UpLoadFile*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdSize(ConvertDWORDToWORD(PageSize));
	msg->SetCmdType(GetMsgType(Main_Ftp, CF_UpLoadFile));
	msg->FileSum = FileSize;
	fread_s(msg->Array, FileSize,1, FileSize, pFile);
	fclose(pFile);
	DWORD SrcLength = CountArray(msg->FileName);
	DWORD DestLength = strlen(FileName);
	strncpy_s(msg->FileName, SrcLength, FileName, DestLength);

	std::vector<CF_Cmd_UpLoadFile*> pVec;
	SqlitMsg(msg, PageSize, FileSize, false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CF_Cmd_UpLoadFile*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			g_FishServer.SendNetCmdToFtpServer(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}