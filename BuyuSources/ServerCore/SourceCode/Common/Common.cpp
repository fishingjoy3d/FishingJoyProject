#include "stdafx.h"
#include "Common.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
struct GlobalInit
{
	GlobalInit()
	{
		setlocale(LC_ALL, "");
		timeBeginPeriod(1);
		WSADATA wsa = {0};
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			//???ˉsocket ê§°üá? 
			ASSERT(false);
		}
		
	}
	~GlobalInit()
	{
		timeEndPeriod(1);
		WSACleanup();
	}
};

GlobalInit  g_GlobalInit;
ILogHandler	*g_Handler = new LogHandler();
WCHAR		g_SprintBuffer[4096];

void SetLogHandler(ILogHandler *handler)
{
	g_Handler = handler;
}
const WCHAR* Sprintf(const WCHAR *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	vswprintf_s(g_SprintBuffer, sizeof(g_SprintBuffer) / sizeof(g_SprintBuffer[0]), pcStr, var);
	va_end(var);
	return g_SprintBuffer;
}

void Log(const WCHAR *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscwprintf(pcStr, var);
	WCHAR *pBuffer = new WCHAR[nCount + 1];
	if (pBuffer)
	{
		vswprintf_s(pBuffer, nCount + 1, pcStr, var);
		if (g_Handler == null)
		{
			wcout << pBuffer << endl;
			OutputDebugStringW(pBuffer);
			OutputDebugStringW(L"\n");
		}
		else
			g_Handler->LogHandlerW(pBuffer);

		SAFE_DELETE_ARR(pBuffer);
	}
	va_end(var);
}

void Log(const CHAR *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	UINT nCount = _vscprintf(pcStr, var);
	CHAR *pBuffer = new CHAR[nCount + 1];
	if (pBuffer)
	{
		vsprintf_s(pBuffer, nCount + 1, pcStr, var);
		if (g_Handler == null)
		{
			wcout << pBuffer << endl;
			OutputDebugStringA(pBuffer);
			OutputDebugStringA("\n");
		}
		else
			g_Handler->LogHandlerA(pBuffer);

		SAFE_DELETE_ARR(pBuffer);
	}
	va_end(var);
}
void InitSocket(SOCKET s, UINT sendBuffSize, UINT recvBuffSize, bool bTCP)
{
	unsigned long arg = 1;
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char*)&recvBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char*)&sendBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
	if (bTCP)
	{
		setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	}
	ioctlsocket(s, FIONBIO, &arg);
	static struct linger lingerSocket;
	lingerSocket.l_onoff = 1;
	lingerSocket.l_linger = 3;
	//setsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&lingerSocket, sizeof(lingerSocket));
}
bool CreateSocket(UINT type, USHORT port, uint recvBuffSize, uint sendBuffSize, CreateSocketData &csd, UINT localIP)
{
	SOCKET s = INVALID_SOCKET;
	if (type & CST_UDP)
	{
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else if (type & CST_TCP_IOCP)
	{
		s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}
	else if (type & CST_TCP)
	{
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	if (s == INVALID_SOCKET)
		return false;

	unsigned long arg = 1;
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const char*)&recvBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (const char*)&sendBuffSize, sizeof(uint));
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
	if (type & CST_UDP)
	{
		//setsockopt(s, IPPROTO_UDP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	}
	else
	{
		setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	}
	ioctlsocket(s, FIONBIO, &arg);
	if (type & CST_BIND)
	{
		SOCKADDR_IN addr;
		memset(&addr, 0, sizeof(addr));
		if (localIP == 0)
			addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		else
			addr.sin_addr.S_un.S_addr = localIP;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		int addrSize = sizeof(addr);
		int ret = ::bind(s, (SOCKADDR*)&addr, sizeof(addr));
		if (ret != 0)
		{
			closesocket(s);
			Log("Bind Failed:%d", WSAGetLastError());
			return false;
		}
		ret = ::getsockname(s, (SOCKADDR*)&addr, &addrSize);
		if (ret != 0)
		{
			closesocket(s);
			Log("Getsockname Failed:%d", WSAGetLastError());
			return false;
		}
		if (localIP != 0 && addr.sin_addr.S_un.S_addr != localIP)
		{
			Log("绑定本机指定IP失败.");
			return false;
		}
		csd.Port		= ntohs(addr.sin_port);
		csd.Addr		= addr;
		csd.AddrSize	= addrSize;
	}
	csd.Socket = s;
	return true;
}
const char *TRANS_BEGIN = "transdata=";
const int TRANS_COUNT = strlen(TRANS_BEGIN);
const char *SIGN_STR = "sign=";
const int SIGN_COUNT = strlen(SIGN_STR);
const UINT SIGN_CRC = AECrc32("sign", 4, 0);
bool SplitStrByIpay(const char *pcstr, UINT size, HashMap<UINT, std::string> & strList)
{
	const char *pcend = pcstr + size;
	if (strncmp(pcstr, TRANS_BEGIN, TRANS_COUNT) == 0)
	{
		pcstr += TRANS_COUNT;
	}
	while (pcstr < pcend)
	{
		while ((*pcstr == '"' || *pcstr == ',' || *pcstr=='{') && pcstr < pcend)
			++pcstr;
		//key;
		string key, value;
		const char *ps = pcstr;
		while (ps < pcend)
		{
			if (*ps == '"')
				break;
			else
				++ps;
		}
		if (*ps != '"')
			break;

		key.append(pcstr, ps - pcstr);

		//value;
		pcstr = ps + 2;
		if (*pcstr == '"')
			++pcstr;
		if (pcstr >= pcend)
			break;
		ps = pcstr;
		while (ps < pcend)
		{
			if (*ps == '"' || *ps == ',' || *ps == '}')
				break;
			else
				++ps;
		}
		if (ps == pcend)
			break;
		value.append(pcstr, ps - pcstr);

		//Log("Key:%s, Value:%s", key.c_str(), value.c_str());
		for (UINT i = 0; i < key.size(); ++i)
			((char*)key.c_str())[i] = ::tolower(key.c_str()[i]);
		UINT crc = AECrc32(key.c_str(), key.size(), 0);
		strList.insert(make_pair(crc, value));
		pcstr = ps;
		if (*pcstr == '}')
		{
			++pcstr;
			break;
		}
	}
	while (*pcstr == '}' || *pcstr == '&')
		++pcstr;
	if (pcstr < pcend && strncmp(pcstr, SIGN_STR, SIGN_COUNT) == 0)
	{
		pcstr += SIGN_COUNT;
		if (pcstr >= pcend)
			return false;
		const char *ps = strchr(pcstr, '&');
		if (ps < pcend)
		{
			string value;
			value.append(pcstr, ps - pcstr);
			//Log("Key:sign, Value:%s", value.c_str());
			strList.insert(make_pair(SIGN_CRC, value));
		}
	}
	return true;
}