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

	struct Point {
		WORD index_from_col;
		WORD index_from_row;
	};

	// ���� ���� ȣȯ��
	RSDLL HANDLE CreateEdit(int maxChars, const int unknown);
	RSDLL int SetIME();
	RSDLL void FocusWindow();
	RSDLL BOOL ReleaseIME();
	RSDLL int GetCharText(const wchar_t* buffers);

	// �߰� API
	RSDLL WCHAR* GetCharFromPos(const int x, const int y);
	RSDLL LPWSTR GetLastChar();

#ifdef __cplusplus
}
#endif