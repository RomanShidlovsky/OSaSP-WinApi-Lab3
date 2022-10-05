#pragma once

#include <Windows.h>

#ifdef DRAWDLL_EXPORTS
#define DRAWDLL_API __declspec(dllexport)
#else
#define DRAWDLL_API __declspec(dllimport)
#endif

extern "C" DRAWDLL_API VOID __stdcall Draw(HDC hdc, int x, int y, int radius);