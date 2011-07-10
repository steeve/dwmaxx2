#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "..\\dwmaxx\\dwmaxx.h"
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")

#ifdef _WIN64
    #ifdef _DEBUG
        #pragma comment(lib, "..\\x64\\Debug\\dwmaxx64.lib")
    #else
        #pragma comment(lib, "..\\x64\\Release\\dwmaxx64.lib")
    #endif
#else
    #ifdef _DEBUG
        #pragma comment(lib, "..\\Debug\\dwmaxx32.lib")
    #else
        #pragma comment(lib, "..\\Release\\dwmaxx32.lib")
    #endif
#endif

int main(int argc, char **argv)
{
    HWND win = NULL;
    do
    {
        win = FindWindowEx(NULL, win, "Notepad", NULL);
    } while (GetWindowTextLength(win) == 0);

    DWMWINDOWATTRIBUTE props[] = {
    DWMWA_NCRENDERING_ENABLED,
    DWMWA_NCRENDERING_POLICY,
        DWMWA_TRANSITIONS_FORCEDISABLED,
        DWMWA_ALLOW_NCPAINT,
        DWMWA_CAPTION_BUTTON_BOUNDS,
        DWMWA_NONCLIENT_RTL_LAYOUT,
        DWMWA_FORCE_ICONIC_REPRESENTATION,
        DWMWA_FLIP3D_POLICY,
        DWMWA_EXTENDED_FRAME_BOUNDS,
        DWMWA_HAS_ICONIC_BITMAP,
        DWMWA_DISALLOW_PEEK,
        DWMWA_EXCLUDED_FROM_PEEK,
        DWMWA_LAST,
        (DWMWINDOWATTRIBUTE)NULL
    };
    int i = 0;
    DWMNCRENDERINGPOLICY pol = DWMNCRP_ENABLED;
    HRESULT hr = DwmSetWindowAttribute(win, DWMWA_NCRENDERING_POLICY, (void*)&pol, sizeof(DWMNCRENDERINGPOLICY));

    hr = S_OK;

    //HANDLE texHandle = DwmaxxGetWindowSharedHandle((HWND)win);

    //printf("HWND=0x%I64x\n", win);
    //printf("HANDLE=0x%I64x\n", texHandle);

    //_getch();

    //HWND hWnd = FindWindow("SpotifyMainWindow", NULL);
    //HWND hWnd = FindWindow("Notepad", NULL);
    //WriteWatermark(hWnd);
    //RECT rect;

    //HRGN region = CreateRectRgn(0, 0, 100, 100);

    HDC hdc = GetDCEx(win, NULL, DCX_WINDOW | DCX_PARENTCLIP | DCX_CACHE);

    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++)
            SetPixel(hdc, x, y, RGB(0, 0, 0xFF));
    //BOOL visible = PtVisible(hdc, 0, 0);
    //COLORREF color = SetPixel(hdc, 0, 0, RGB(0xDE, 0xAD, 0xC0));
    //color = GetPixel(hdc, 0, 0);
    //ReleaseDC(hWnd, hdc);
    //WriteWatermark(FindWindow("Notepad", NULL));
    //RECT rect;
    //rect.top = rect.left = 0;
    //rect.right = rect.bottom = 1;
    //InvalidateRect(NULL, &rect, FALSE);
    /*HWND rpcHwnd = FindWindow(DWMEX_RPC_WINDOW_CLASS, NULL);
    LRESULT r = SendMessage(rpcHwnd, DWMEX_GET_SHARED_HANDLE, (WPARAM)0xB031C, NULL);
*/
    return (0);
}
