#include "stdafx.h"
#include "Dump.h"
#include <strsafe.h>
#include <dbghelp.h>
#pragma warning(disable:4191) 
int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers)
{
	BOOL bOwnDumpFile = FALSE;
	HANDLE hDumpFile = hFile;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;
	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary(L"DbgHelp.dll");
	if (hDbgHelp)
	{
		pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	}
	if (pfnMiniDumpWriteDump)
	{
		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
		{
			//TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR szFileName[MAX_PATH] = { 0 };

			TCHAR* szVersion = L"v1.0";
			//TCHAR dwBufferSize = MAX_PATH;
			SYSTEMTIME stLocalTime;
			GetLocalTime(&stLocalTime);

			StringCchPrintf(szFileName, MAX_PATH, L"%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", szVersion,
				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
				GetCurrentProcessId(), GetCurrentThreadId());
			hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
			bOwnDumpFile = TRUE;
			OutputDebugString(szFileName);
		}
		if (hDumpFile != INVALID_HANDLE_VALUE)
		{
			ExpParam.ThreadId = GetCurrentThreadId();
			ExpParam.ExceptionPointers = pExceptionPointers;
			ExpParam.ClientPointers = FALSE;
			pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);
			if (bOwnDumpFile)
			{
				CloseHandle(hDumpFile);
			}
		}
	}
	if (hDbgHelp != NULL)
		FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}
LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;
	return GenerateMiniDump(NULL, lpExceptionInfo);
}
Dump::Dump()
{
}
void Dump::OnInit()
{
	SetUnhandledExceptionFilter(ExceptionFilter);
}
Dump::~Dump()
{

}