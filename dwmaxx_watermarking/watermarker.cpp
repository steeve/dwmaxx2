#include <Windows.h>

#include <Windows.h>
#include <Windows.h>
#include "globals.h"
#include "watermarker.h"
#include "hooks.h"
#include "constants.h"

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

void    DwmaxxInstallHooks()
{
    // no real need for prolog after all...
    //g_wndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WndProcProlog, g_hInstance, 0);
    g_wndProcRetHook = SetWindowsHookEx(WH_CALLWNDPROCRET, WndProcEpilog, g_hInstance, 0);
    g_shellHook = SetWindowsHookEx(WH_SHELL, ShellProcProlog, g_hInstance, 0);
}

void    DwmaxxRemoveHooks()
{
    UnhookWindowsHookEx(g_wndProcHook);
    UnhookWindowsHookEx(g_wndProcRetHook);
    UnhookWindowsHookEx(g_shellHook);
}
