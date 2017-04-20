#include "stdafx.h"
#include "FishCallbak.h"
#include "FishScene\FishResManager.h"
//#include "FishRoom.h"
#pragma comment(lib, "D3DX9.lib")

WCHAR g_StrBuffer[1024];
float RADIN = 3.1415926535f / 180.0f;
float InvShortMaxValue = 1.0f / SHRT_MAX;
float InvUshortMaxValue = 1.0f / USHRT_MAX;

//#if SIMPLE_SERVER_MODE
//	TCPServer *g_pTCPServer = NULL;
//#endif
//
const WCHAR* InnerSprintf(const WCHAR *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	vswprintf_s(g_StrBuffer, sizeof(g_StrBuffer) / sizeof(WCHAR), pcStr, var);
	va_end(var);
	return g_StrBuffer;
}
void InnerFishLog(const WCHAR *pcStr, ...)
{
	va_list var;
	va_start(var, pcStr);
	int nCount = _vscwprintf(pcStr, var);
	WCHAR *pBuffer = new WCHAR[nCount + 1];
	if (pBuffer)
	{
		vswprintf_s(pBuffer, nCount + 1, pcStr, var);
		FishCallback::FishLog(pBuffer);
		SAFE_DELETE_ARR(pBuffer);
	}
	va_end(var);
}

bool InitFishData(const WCHAR *pcDir)
{
	setlocale(LC_ALL, "");
	//Camera cam;
	//cam.CreateProj(CAMERA_FOV * RADIAN, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	//Frustum::Inst()->Init(&cam);

	bool bRet = FishResManager::Inst()->Init(pcDir);
//#if SIMPLE_SERVER_MODE
//	g_pTCPServer = new TCPServer();
//	if (!g_pTCPServer->Init(SOCKET_PORT))
//		return false;
//#endif
	return bRet;
}
void ReleaseFishData()
{
//#if SIMPLE_SERVER_MODE
//	g_pTCPServer->Shutdown();
//	SAFE_DELETE(g_pTCPServer);
//
//	
//#endif
	FishResManager::Inst()->Shutdown();
}