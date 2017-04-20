#pragma once
#define PING_FAILED 0xffffffff
#include <WS2tcpip.h>
class TCPPing
{
public:
	static UINT Ping(const char *ip, USHORT port, UINT timeout)
	{
		SOCKET s = INVALID_SOCKET;
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
			return PING_FAILED;
		unsigned long arg = 1;
		setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
		ioctlsocket(s, FIONBIO, &arg);
		
		SOCKADDR_IN addr;
		int addrSize = sizeof(addr);
		memset(&addr, 0, sizeof(addr));
		inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		UINT tick = timeGetTime();
		int ret = connect(s, (sockaddr*)&addr, addrSize);
		if (ret != 0 && WSAGetLastError() != 10035)
		{
			closesocket(s);
			return PING_FAILED;
		}
		char buff[128];
		while (timeGetTime() - tick < timeout)
		{
			int ret = recv(s, buff, sizeof(buff), 0);
			if (ret > 0)
			{
				tick = timeGetTime() - tick;
				closesocket(s);
				return tick;
			}
			Sleep(1);
		}
		closesocket(s);
		return timeout;
	}
};