//// WechatDllCpp.cpp : 定义 DLL 应用程序的导出函数。
////
//
//#include "stdafx.h"
//#include <stdio.h>
//#include "resource1.h"
//#include "malloc.h"
//
////WeChatWin.dll + 125C250 微信号
////WeChatWin.dll + 125C3CC 头像
////WeChatWin.dll + 125C120 电话
////WeChatWin.dll + 125C1F0 城市
////WeChatWin.dll + 125C1D8 省份
////WeChatWin.dll + 125C108 邮箱
////WeChatWin.dll + 125C0EC 昵称
////WeChatWin.dll + 125C2c8 国家
//
//DWORD getWechatWinAddr()
//{
//	HMODULE winaddr = LoadLibrary(L"WeChatWin.dll");
//
//	return (DWORD)winaddr;
//}
//
//wchar_t * UTF8ToUnicode(const char* str)
//{
//	int    textlen = 0;
//	wchar_t * result;
//	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
//	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
//	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
//	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
//	return    result;
//}
//
//void readData(HWND dlg_box)
//{
//	DWORD wechatWin = getWechatWinAddr();
//
//	char * wx_id[0x100] = {0};
//
//	//sprintf_s(wx_id, L"%s", wechatWin + 0x125C250);
//	//SetDlgItemText(dlg_box, EDT_WXID, UTF8ToUnicode(wx_id));
//}
//
//struct stru_wx
//{
//	wchar_t * _str;
//	int str_len;
//	int str_len2;
//};
//
//void send(wchar_t * text, wchar_t * wx_id)
//{
//	DWORD send_call = getWechatWinAddr() + 0x2E3E20;
//
//	stru_wx s_wxid = {0};
//	s_wxid._str = wx_id;
//	s_wxid.str_len = wcslen(wx_id);
//	s_wxid.str_len2 = wcslen(wx_id) * 2;
//
//	stru_wx s_wxcontent = { 0 };
//	s_wxid._str = text;
//	s_wxid.str_len = wcslen(text);
//	s_wxid.str_len2 = wcslen(text) * 2;
//
//	char buff[0x81c] = { 0 };
//
//	char * w_id = (char *)&s_wxid._str;
//	char * w_content = (char *)&s_wxcontent._str;
//	wchar_t test[0x100] = {0};
//	//w_id = (char *)"wsbblyy";
//	//w_content = (char *)"haha";
//
//	swprintf_s(test, L"module=%p wxid=%p message=%p", send_call, wx_id, w_content);
//	MessageBox(NULL, test, L"error", 0);
//	//return;
//	//MessageBox(NULL, "w_id:"+wchar_t(w_id)+ wchar_t(" w_content:")+wchar_t(w_content), L"error", 0);
//	__asm {
//		mov edx, w_id
//		mov eax, 0x0
//		push 0x1
//		push eax
//		mov ebx, w_content
//		push ebx
//		lea ecx, buff
//		call send_call
//		add esp, 0xC
//
//		//mov edx, w_id
//		//push edx
//		//push 0x1
//		//push 0x0
//		//mov ebx, w_content
//		//push ebx
//		//lea ecx, buff
//		//call send_call
//		//add esp, 0xC
//
//	}
//}
//
//
//
//void writeData()
//{
//	DWORD wechatWin = getWechatWinAddr();
//
//	char content[0x100] = {"sweety"};
//
//	WriteProcessMemory(GetCurrentProcess(), (LPVOID)0x00edbd00, content, sizeof(content), NULL);
//}

// WechatSendMessage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Windows.h>
#include "resource.h"
#include <stdio.h>
#include "malloc.h"

//消息结构体
struct wechatText
{
	wchar_t * pStr;
	int strLen;
	int iStrLen;
	int fill = 0;
	int fill2 = 0;
};

//获取模块基址
DWORD getModuleAddress()
{
	return (DWORD)LoadLibrary(L"WeChatWin.dll");
}


/*
5B8B2128  |.  8B55 CC       |mov edx,[local.13]                      ;  wxid 结构体
5B8B212B  |.  8D43 14       |lea eax,dword ptr ds:[ebx+0x14]
5B8B212E  |.  6A 01         |push 0x1                                ;  0x1
5B8B2130  |.  50            |push eax                                ;  0x0 如果是@那么需要一个结构体指针指向一个wxid
5B8B2131  |.  53            |push ebx                                ;  发送的消息内容结构体被@人wxid
5B8B2132  |.  8D8D E4F7FFFF |lea ecx,[local.519]                     ;  0x81C缓冲区
5B8B2138  |.  E8 A31F2100   |call WeChatWi.5BAC40E0
5B8B213D  |.  83C4 0C       |add esp,0xC
*/
//发送文本消息函数
VOID SendTextMessage(wchar_t * wxid, wchar_t * message)
{
	//2D40E0
	wechatText pWxid = { 0 };
	pWxid.pStr = wxid;
	pWxid.strLen = wcslen(wxid);
	pWxid.iStrLen = wcslen(wxid) * 2;


	wechatText pMessage = { 0 };
	pMessage.pStr = message;
	pMessage.strLen = wcslen(message);
	pMessage.iStrLen = wcslen(message) * 2;
	char* asmWxid = (char*)&pWxid.pStr;
	char* asmMsg = (char*)&pMessage.pStr;

	/*wchar_t test[0x100] = {0};
	swprintf_s(test, "asmMsg:%p", asmMsg);
	MessageBox(NULL, test, L"error", 0);
	return;*/

	//缓冲区
	char buff[0x81C] = { 0 };

	

	//call地址
	DWORD callAdd = getModuleAddress() + 0x2EB4E0;
	__asm {
		mov edx, asmWxid
		mov eax, 0x0
		push 0x1
		push eax
		mov ebx, asmMsg
		push ebx
		lea ecx, buff
		call callAdd
		add esp, 0xC
	}
	
}


/*
5B8B2234  |> \53            |push ebx                                ;  图片地址; Case 2 of switch 5B8B2088
5B8B2235  |.  FF75 CC       |push [local.13]                         ;  wxid
5B8B2238  |.  8D85 A4FBFFFF |lea eax,[local.279]
5B8B223E  |.  50            |push eax                                ;  0x45C 缓冲区
5B8B223F  |.  51            |push ecx                                ;  0x0
5B8B2240  |.  E8 3B80FBFF   |call WeChatWi.5B86A280
5B8B2245  |.  83C4 04       |add esp,0x4
*/
//测试总的发消息call C2010
//VOID sendPicMessage(wchar_t * wxid, wchar_t * picPath)
//{
//	wechatText pWxid = { 0 };
//	pWxid.pStr = wxid;
//	pWxid.strLen = wcslen(wxid);
//	pWxid.iStrLen = wcslen(wxid) * 2;
//
//	wchar_t test[0x100] = { L"D:\\code\\react_native\\jobPda\\app\\Images\\add.png" };
//	wechatText pPic = { 0 };
//	pPic.pStr = test;
//	pPic.strLen = wcslen(test);
//	pPic.iStrLen = wcslen(test) * 2;
//
//	char* asmWxid = (char*)&pWxid.pStr;
//	char* asmPic = (char*)&pPic.pStr;
//
//	DWORD callAdd = getModuleAddress() + 0x7A280;
//	char testa[0x100] = { 0 };
//	char buff[0x45C] = { 0 };
//	sprintf_s(testa, "输入框地址:%p  写死地址:%p  缓冲区地址：%p wxid:%p path:%p", picPath, test, buff, asmWxid, asmPic);
//	MessageBox(NULL, testa, "aa", 0);
//
//	__asm {
//		mov ebx, asmPic
//		push ebx
//		push asmWxid
//		lea eax, buff
//		push eax
//		push 0x0
//		call callAdd
//		add esp, 0x4
//	}
//}

//修改好友备注
VOID editNick(wchar_t * wxid, wchar_t * nick)
{
	__asm {
		push 0x0

	}
}