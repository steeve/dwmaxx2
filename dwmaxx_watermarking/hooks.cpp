#include <Windows.h>
#include "globals.h"
#include "watermarker.h"
#include "..\\dwmaxx\\dwmaxx.h"

LRESULT CALLBACK WndProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT       *msg = (CWPSTRUCT *)lParam;

    return CallNextHookEx(g_wndProcHook, nCode, wParam, lParam);
}


LRESULT CALLBACK WndProcEpilog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPRETSTRUCT    *msg = (CWPRETSTRUCT *)lParam;

    switch (msg->message)
    {
    case WM_SHOWWINDOW:
        if ((BOOL)msg->wParam == FALSE)
        {
            DwmaxxRemoveWindow((HWND)msg->hwnd);
            break;
        }   
    case WM_EXITSIZEMOVE:
    case WM_NCPAINT:
    case WM_PAINT:
        if (IsWindowVisible(msg->hwnd))
            WriteWatermark(msg->hwnd);
        break;
    }

    return CallNextHookEx(g_wndProcRetHook, nCode, wParam, lParam);
}

LRESULT CALLBACK ShellProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case HSHELL_WINDOWREPLACED:
    case HSHELL_WINDOWCREATED:
        if (IsWindowVisible((HWND)wParam))
            WriteWatermark((HWND)wParam);
        break;
    case HSHELL_WINDOWDESTROYED:
        DwmaxxRemoveWindow((HWND)wParam);
        break;
    }

    return CallNextHookEx(g_shellHook, nCode, wParam, lParam);
}
