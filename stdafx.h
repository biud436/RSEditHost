// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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

	// 이전 버전 호환용
	RSDLL HANDLE CreateEdit(int maxChars, const int unknown);
	RSDLL int SetIME(int maxChars = DEFAULT_MAX_CHARS);
	RSDLL void FocusWindow();
	RSDLL BOOL ReleaseIME();
	RSDLL int GetCharText(const wchar_t* buffers);

	RSDLL int GetCaretIndex();

	// 추가 API
	RSDLL LPWSTR GetLastChar();
	RSDLL LPWSTR GetCompStr();

	RSDLL LONG GetTextWidth(LPCWSTR lpString, int c);

#ifdef __cplusplus
}
#endif