#include "pch.h"
#include "DrawDll.h"
#include <math.h>

#define M_PI       3.14159265358979323846   // pi

VOID Draw(HDC hdc, int x, int y, int radius)
{
	int const n = 11;
	double angle = 0;

	POINT points[n+1];

	for (int i = 0; i < n + 1; i++)
	{
		points[i].x = radius * cos(angle * M_PI / 180.0) + x;
		points[i].y = radius * sin(angle * M_PI / 180.0) + y;
		angle = angle + 360.0 / n;
	}

	POINT oldPoint;
	MoveToEx(hdc, points[0].x, points[0].y, &oldPoint);

	for (int i = 1; i < n + 1; i++)
	{
		LineTo(hdc, points[i].x, points[i].y);
		MoveToEx(hdc, points[i].x, points[i].y, NULL);
	}

	MoveToEx(hdc, oldPoint.x, oldPoint.y, NULL);
}