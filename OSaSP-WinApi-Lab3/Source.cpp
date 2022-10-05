#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include <cstring>;
#include <stdio.h>
#include "DrawDll.h"

const LPCTSTR path = L"./Plugins/*.dll";

typedef VOID __stdcall TDraw(HDC hdc, int x, int y, int radius);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE hInstance, int nCmdShow);
VOID PaintL(HWND hWnd, LPPAINTSTRUCT lpPs);
VOID PaintI(HWND hWnd, LPPAINTSTRUCT lpPs);
VOID SetNextDrawFunction();
VOID SetDrawFunction();

HANDLE hWnd;
HMODULE hModule;
TDraw* draw;
HANDLE hFind;
WIN32_FIND_DATAW fileData;
POINT figurePos;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MyRegisterClass(hInstance);
	hWnd = InitInstance(hInstance, nCmdShow);
	if (!hWnd) 
	{
		return FALSE;
	}
	
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH hBrush = CreateSolidBrush(RGB(65, 105, 225));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW || CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWindow";
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		figurePos.x = (clientRect.right - clientRect.left) / 2;
		figurePos.y = (clientRect.bottom - clientRect.top) / 2;
		
		hFind = FindFirstFile(path, &fileData);
		if (hFind)
		{
			SetDrawFunction();
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		PaintI(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		figurePos.x = LOWORD(lParam);
		figurePos.y = HIWORD(lParam);
		SetNextDrawFunction();
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	
	case WM_DESTROY:
		if (hModule)
		{
			FreeLibrary(hModule);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindow(L"MainWindow", L"Hey, wake up!",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

VOID SetNextDrawFunction()
{
	draw = NULL;
	FindNextFile(hFind, &fileData);
	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		SetDrawFunction();
	}

	if (!draw)
	{
		hFind = FindFirstFile(path, &fileData);
		if (hFind)
		{
			SetDrawFunction();
		}
	}
}

VOID SetDrawFunction()
{	
	const wchar_t dir[11] = L"./Plugins/";
	wchar_t fullPath[280];
	wcscpy_s(fullPath, dir);
	wcscat_s(fullPath, fileData.cFileName);

	hModule = LoadLibrary(fullPath);
	if (hModule)
	{
		draw = draw = (TDraw*)GetProcAddress(hModule, "Draw");
	}
	

	while (!draw)
	{
		FindNextFile(hFind, &fileData);
		if (GetLastError() == ERROR_NO_MORE_FILES)
		{
			break;
		}
		if (hModule)
		{
			FreeLibrary(hModule);
		}
		wcscpy_s(fullPath, dir);
		wcscat_s(fullPath, fileData.cFileName);
		hModule = LoadLibrary(fullPath);
		if (hModule)
		{
			draw = (TDraw*)GetProcAddress(hModule, "Draw");
		}	
	}
}

VOID PaintL(HWND hWnd, LPPAINTSTRUCT lpPs)
{
	if (draw)
	{
		draw(lpPs->hdc, figurePos.x, figurePos.y, 100);
	}	
}

VOID PaintI(HWND hWnd, LPPAINTSTRUCT lpPs)
{
	Draw(lpPs->hdc, figurePos.x, figurePos.y, 100);
}

