#pragma once

//void readData(HWND dlg_box);
//void writeData();
//void send(wchar_t * text, wchar_t * wx_id);
wchar_t * UTF8ToUnicode(const char* str);
void SendTextMessage(wchar_t * wxid, wchar_t * message);