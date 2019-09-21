// RSEditHost.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#define _RSDLL
#include "stdafx.h"
#include <tchar.h>
#include <sstream>
#include <string>

#pragma comment(lib, "imm32.lib")

HWND g_hWnd = NULL;
HWND g_hEdit = NULL;
BOOL g_bInit = FALSE;

WNDPROC OldWndProc;
TCHAR g_lpszLastChar[3] = { 0x0A, 0x0C, 0 };
LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

RSDLL HANDLE CreateEdit(int maxChars, const int unknown)
{
	// Find the Window Handle from RGSS Player.
	WCHAR lpszGameTitle[MAX_PATH];
	WCHAR lpszIniFile[MAX_PATH];

	GetCurrentDirectoryW(MAX_PATH, lpszIniFile);

	std::wstringstream wss;
	wss << lpszIniFile << L"\\Game.ini";
	std::wstring sIniFile = wss.str();

	GetPrivateProfileStringW(L"Game", L"Title", L"Untitled", lpszGameTitle, MAX_PATH, &sIniFile[0]);

	g_hWnd = FindWindowW(L"RGSS Player", lpszGameTitle);

	// Get the hInstance
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLong(g_hWnd, GWLP_HINSTANCE));

	// Create EDIT
	g_hEdit = CreateWindow(TEXT("edit"), TEXT(""),
		WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_WANTRETURN,
		0, 0, 200, 100, g_hWnd, (HMENU)100, hInstance, NULL);
	ShowWindow(g_hEdit, SW_HIDE);

	OldWndProc = (WNDPROC)SetWindowLongPtr(g_hEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);

	// 입력 글자 수 제한
	SendMessage(g_hEdit, EM_LIMITTEXT, static_cast<WPARAM>(maxChars), 0);

	if (g_hEdit)
		g_bInit = TRUE;

	return g_hEdit;
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HIMC hImc = NULL;
	TCHAR *szComp;

	switch (iMessage) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			SetFocus(g_hWnd);
			break;
		}
		break;
	case WM_IME_STARTCOMPOSITION: // 조립 시작
		break;
	case WM_IME_ENDCOMPOSITION: // 조립 끝
		break;
	case WM_IME_COMPOSITION: // 조립 상태

		hImc = ImmGetContext(hWnd);

		if (lParam & GCS_COMPSTR) // 조립 중
		{
			//
		}
		else  if (lParam & GCS_RESULTSTR) // 조립 완료
		{
			int len = ImmGetCompositionString(hImc, GCS_RESULTSTR, NULL, 0);
			szComp = new TCHAR[len + 1];
			ImmGetCompositionString(hImc, GCS_RESULTSTR, szComp, len);
			szComp[len] = 0;
			lstrcpy(g_lpszLastChar, szComp);
			SetIME();
			delete[] szComp;
		}

		ImmReleaseContext(hWnd, hImc);

		break;
	case WM_SETFOCUS: // 포커스 획득
		break;
	case WM_KILLFOCUS: // 포커스 상실
		break;
	}
	return CallWindowProc(OldWndProc, hWnd, iMessage, wParam, lParam);
}

RSDLL int SetIME()
{
	if (!g_bInit)
		return FALSE;

	SetFocus(g_hEdit);
	return TRUE;
}

RSDLL void FocusWindow()
{
	if (!g_bInit)
		return;

	SetFocus(g_hWnd);
}

RSDLL BOOL ReleaseIME()
{
	if (!g_bInit)
		return FALSE;

	SetWindowLongPtr(g_hEdit, GWLP_WNDPROC, (LONG_PTR)OldWndProc);
	BOOL ret = DestroyWindow(g_hEdit);
	SetFocus(g_hWnd);
	
	g_bInit = FALSE;
	g_hEdit = NULL;
	g_hWnd = NULL;

	return ret;
}

RSDLL int GetCharText(const wchar_t* buffers)
{
	if (!g_bInit)
		return FALSE;

	SendMessage(g_hEdit, WM_GETTEXT, 128, (LPARAM)buffers);

	return TRUE;
}

RSDLL WCHAR* GetCharFromPos(const int x, const int y)
{
	Point pt = { 0, 0 };
	WCHAR lpszText[MAX_PATH];

	DWORD ret = SendMessage(g_hEdit, EM_CHARFROMPOS, 0, MAKEWORD(x, y));
	
	SendMessage(g_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)lpszText);

	pt.index_from_col = LOWORD(ret);
	pt.index_from_row = HIWORD(ret);

	return &lpszText[pt.index_from_col];
}

RSDLL LPWSTR GetLastChar()
{
	return g_lpszLastChar;
}