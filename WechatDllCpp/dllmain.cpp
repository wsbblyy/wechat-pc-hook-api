// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "WechatDllCpp.h"
#include <stdio.h>
#include "resource1.h"
#include <Windows.h>
#include "httpServer.h"

INT_PTR CALLBACK Dlgproc(
	HWND Arg1,
	UINT Arg2,
	WPARAM Arg3,
	LPARAM Arg4
);

DWORD ThreadProc(HMODULE hModule);
DWORD ThreadProc_http();
DWORD ThreadProc_redis();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//DialogBox(hModule, MAKEINTRESOURCE(DLG_MAIN), NULL, &Dlgproc);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc_http, hModule, 0, NULL);
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

INT_PTR CALLBACK Dlgproc(
	HWND Arg1,
	UINT Arg2,
	WPARAM Arg3,
	LPARAM Arg4
)
{

	wchar_t wxid[0x100] = { 0 };
	wchar_t message[0x100] = { 0 };

	switch (Arg2)
	{
	case WM_INITDIALOG:
		break;
	case WM_CLOSE:
		EndDialog(Arg1, 0);
	case WM_COMMAND:
		if (Arg3 == BTN_SEND)
		{
			//wchar_t wxid[0x100] = { 0 };
			//wchar_t message[0x100] = { 0 };
			//GetDlgItemText(Arg1 ,EDT_INPUT, message, sizeof(message));
			//GetDlgItemText(Arg1, EDT_WXID, wxid, sizeof(wxid));
			//send(message, wxid);

			GetDlgItemText(Arg1, EDT_WXID, wxid, sizeof(wxid));
			GetDlgItemText(Arg1, EDT_INPUT, message, sizeof(message));
			SendTextMessage(wxid, message);
		}

		if (Arg3 == BTN_WRITE)
		{
			
		}

		break;
	}

	return false;
}

DWORD ThreadProc(HMODULE hModule)
{
	DialogBox(hModule, MAKEINTRESOURCE(DLG_MAIN), NULL, &Dlgproc);
	return TRUE;
}

DWORD ThreadProc_redis()
{
	return 0;
}

DWORD ThreadProc_http()
{
	httpServer();
	return TRUE;
}

