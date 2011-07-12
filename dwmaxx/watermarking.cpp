#include <Windows.h>
#include "watermarking.h"

void    WriteWatermark(HWND hWnd)
{
    HDC hdc = GetDCEx(hWnd, NULL, DCX_WINDOW | DCX_PARENTCLIP | DCX_LOCKWINDOWUPDATE);
    if (hdc != NULL)
    {
        RECT winRect;
        GetWindowRect(hWnd, &winRect);
        DWORD color = 0;
        color |= ((DWORD)hWnd & 0x0000FF) << 16;
        color |= ((DWORD)hWnd & 0x00FF00);
        color |= ((DWORD)hWnd & 0xFF0000) >> 16;
        SetPixel(hdc, (winRect.right - winRect.left) / 2, (winRect.bottom - winRect.top) / 2, (COLORREF)color);
        ReleaseDC(hWnd, hdc);
    }
}
