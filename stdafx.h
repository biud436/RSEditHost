// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#if defined __cplusplus
extern "C" {
#endif

#ifdef _RSDLL
#define RSDLL __declspec(dllexport)
#else
#define RSDLL __declspec(dllimport)
#endif

	#define DEFAULT_MAX_CHARS 40

	struct Point {
		WORD index_from_col;
		WORD index_from_row;
	};

	// ���� ���� ȣȯ��
	RSDLL HANDLE CreateEdit(int maxChars, const int unknown);
	RSDLL int SetIME(int maxChars = DEFAULT_MAX_CHARS);
	RSDLL void FocusWindow();
	RSDLL BOOL ReleaseIME();
	RSDLL int GetCharText(const wchar_t* buffers);

	RSDLL int GetCaretIndex();

	// �߰� API
	RSDLL LPWSTR GetLastChar();
	RSDLL LPWSTR GetCompStr();

	RSDLL LONG GetTextWidth(LPCWSTR lpString, int c);

#ifdef __cplusplus
}
#endif