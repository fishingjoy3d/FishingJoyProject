#pragma once

#define HTTP_CLIENT_BUFF_SIZE	40960
#define HTTP_REQUEST_COUNT		1024
enum RequestState
{
	REQUEST_NONE,
	REQUEST_CREATE,
	REQUEST_CONNECTED,
	REQUEST_SEND,
	REQUEST_RECV,
	REQUEST_FAILED,
	REQUEST_OK,
};
struct HttpClientResult
{
	bool			Orignal;//最原始的字符串发送和接收
	UINT_PTR		ID;
	char			SendBuff[HTTP_CLIENT_BUFF_SIZE];
	char			PostBuff[HTTP_CLIENT_BUFF_SIZE];
	char			RecvBuff[HTTP_CLIENT_BUFF_SIZE];
	uint			SendSize;
	uint			PostSize;
	uint			RecvSize;
	RequestState	State;
	USHORT			HostIdx;
	UINT			Tick;
	SOCKET			Socket;
	UINT			Content_Length;
	UINT			Content_Start;
};
struct HttpClientInitData
{
	UINT		ThreadNum;
	UINT		Timeout;
	UINT		SleepTime;
};
struct HttpHostData
{
	char	Host[128];
	BYTE	HostSize;
	struct in_addr	IP;
	USHORT	Port;
};
#define HOST_IDX_ERR 0xffff
class HttpClient
{
public:
	HttpClient() :m_RequestList(HTTP_REQUEST_COUNT), m_ResponseList(HTTP_REQUEST_COUNT)
	{
		m_bRun = false;
		m_ThreadIdx = 0;
		m_ExitIdx = 0;
	}
	//注册域名，返回域名的索引,失败返回HOST_IDX_ERR
	USHORT AddHost(const char *pcURL, USHORT port = 80);
	//注册IP，返回索引,失败返回HOST_IDX_ERR
	USHORT AddIP(const char *pcIP, USHORT port = 80);
	bool Init(const HttpClientInitData &d);
	void Shutdown();
	//hostIdx域名或者IP的索引
	bool AddRequest(UINT_PTR ID, USHORT hostIdx, const char *pcURL, bool bOrignal=false);
	bool AddPostRequest(UINT_PTR ID, USHORT hostIdx, const char *pcURL, const char *pcPostData, bool orignal = false);
	void _Thread();
	bool GetResult(HttpClientResult* &pResult)
	{
		return m_ResponseList.Pop(pResult);
	}
	void FreeResult(HttpClientResult* pResult)
	{
		delete(pResult);
	}
protected:
	//获取域名的IP地址
	static bool GetIPByName(const char *pcName, char ip[30]);
	CACHELINE(0);
	volatile bool m_bRun;
	CACHELINE(1);
	volatile UINT m_ThreadIdx;
	CACHELINE(2);
	volatile UINT m_ExitIdx;
	CACHELINE(3);

	vector<HttpHostData>			m_HostList;
	SafeArrayMT<HttpClientResult*>	m_RequestList;
	SafeArrayMT<HttpClientResult*>	m_ResponseList;
	HttpClientInitData				m_InitData;
};