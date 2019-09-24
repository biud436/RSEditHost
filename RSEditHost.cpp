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
HMODULE g_hRGSSPlayer = NULL;

WNDPROC OldWndProc;
TCHAR g_lpszLastChar[3] = { 0, 0, 0 };
TCHAR g_lpszCompChar[3] = { 0, 0, 0 };
LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int g_nLastMaxChars = 40;
BOOL g_bHan = FALSE;

int g_nEditIndex = 0;

RSDLL HANDLE CreateEdit(int maxChars, const int unknown)
{
	// Find the Window Handle from RGSS Player.
	WCHAR lpszGameTitle[MAX_PATH];
	WCHAR lpszIniFile[MAX_PATH];
	WCHAR lpszDllPath[MAX_PATH];

	g_nLastMaxChars = maxChars;

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

void ConcatStringToComp(HIMC& hImc, DWORD type, TCHAR change_to[])
{
	int len = ImmGetCompositionString(hImc, type, NULL, 0);
	if (len > 0) 
	{
		TCHAR *character = new TCHAR[len + 1];

		ImmGetCompositionString(hImc, type, character, len);
		character[len] = 0;

		lstrcpy(change_to, character);

		SetIME(g_nLastMaxChars);

		delete[] character;
	}

}

LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HIMC hImc = NULL;
	TCHAR szChar[128];

	switch (iMessage) {
	case WM_CREATE:
		g_bHan = FALSE;
		g_nEditIndex = 0;
		break;
	case WM_CHAR:
		g_bHan = FALSE;
		switch (wParam)
		{
		case VK_BACK:
			if (g_nEditIndex > 0) {
				g_nEditIndex--;
			}
			break;
		default:
			g_nEditIndex++;
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			SetFocus(g_hWnd);
			break;
		case VK_LEFT:
			if (g_nEditIndex > 0) {
				g_nEditIndex--;
			}
			break;
		case VK_RIGHT: 
			GetCharText(szChar);
			if (g_nEditIndex < _tcslen(szChar)) {
				g_nEditIndex++;
			}
			break;
		case VK_HOME:
			g_nEditIndex = 0;
			break;
		case VK_END:
			GetCharText(szChar);
			g_nEditIndex = _tcslen(szChar);
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
			ConcatStringToComp(hImc, GCS_COMPSTR, g_lpszCompChar);
		}
		else  if (lParam & GCS_RESULTSTR) // 조립 완료
		{
			ConcatStringToComp(hImc, GCS_RESULTSTR, g_lpszLastChar);
		}

		ImmReleaseContext(hWnd, hImc);

		break;
	case WM_IME_CHAR:
		//if (IsDBCSLeadByte((BYTE)(wParam >> 8))) {
		//	g_lpszLastChar[0] = HIBYTE(LOWORD(wParam));
		//	g_lpszLastChar[1] = LOBYTE(LOWORD(wParam));
		//	g_lpszLastChar[2] = 0;
		//} else {
		//	g_lpszLastChar[0] = (BYTE)wParam;
		//	g_lpszLastChar[1] = 0;
		//}

		break;
	case WM_SETFOCUS: // 포커스 획득
		break;
	case WM_KILLFOCUS: // 포커스 상실
		break;
	}
	return CallWindowProc(OldWndProc, hWnd, iMessage, wParam, lParam);
}

RSDLL int SetIME(int maxChars)
{
	if (!g_bInit) {
		CreateEdit(maxChars, 0);
	}
		
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
	g_nEditIndex = 0;

	return ret;
}

RSDLL int GetCharText(const wchar_t* buffers)
{
	if (!g_bInit)
		return FALSE;

	SendMessage(g_hEdit, WM_GETTEXT, 128, (LPARAM)buffers);

	return TRUE;
}

RSDLL LPWSTR GetLastChar()
{
	return g_lpszLastChar;
}

RSDLL LPWSTR GetCompStr()
{
	return g_lpszCompChar;
}

RSDLL LONG GetTextWidth(LPCWSTR lpString, int c)
{
	SIZE sz = { 0, 0 };
	
	if (!g_bInit)
		return 0;

	HDC hDC = GetDC(g_hWnd);

	// 폰트 선택이 되지 않았기 때문에 정확한 폰트를 계산할 수 없음.
	// pGraphics-> 어딘가에 있을 것으로 추정.

	GetTextExtentPoint32W(hDC, lpString, c, &sz);

	ReleaseDC(g_hWnd, hDC);

	return sz.cx;
	
}

RSDLL int GetCaretIndex()
{
	return g_nEditIndex;
}

RSDLL void SetEditText(const wchar_t* buffers)
{
	if (!g_bInit)
		return;

	int len = _tcslen(buffers);
	SendMessage(g_hEdit, WM_SETTEXT, 0, (LPARAM)buffers);
	SendMessage(g_hEdit, WM_KEYDOWN, VK_END, 0);
}