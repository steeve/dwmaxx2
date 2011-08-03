#include <Windows.h>
#include <dwmapi.h>
#include "globals.h"
#include "constants.h"
#include "watermarking.h"
#include "dwmaxx.h"
#include "win_hooks.h"

void    DwmaxxInstallHooks()
{
    g_wndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WndProcProlog, g_hInstance, 0);
    g_wndProcRetHook = SetWindowsHookEx(WH_CALLWNDPROCRET, WndProcEpilog, g_hInstance, 0);
    g_shellHook = SetWindowsHookEx(WH_SHELL, ShellProcProlog, g_hInstance, 0);
    g_cbtHook = SetWindowsHookEx(WH_CBT, CBTProc, g_hInstance, 0);
}

void    DwmaxxRemoveHooks()
{
    UnhookWindowsHookEx(g_wndProcHook);
    UnhookWindowsHookEx(g_wndProcRetHook);
    UnhookWindowsHookEx(g_shellHook);
    UnhookWindowsHookEx(g_cbtHook);
}

LRESULT CALLBACK WndProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT       *msg = (CWPSTRUCT *)lParam;

    if (IsTopLevelWindow(msg->hwnd) == TRUE)
    {
        switch (msg->message)
        {
        case WM_SHOWWINDOW:
            if ((BOOL)msg->wParam == FALSE)
                DwmaxxRemoveWindow((HWND)msg->hwnd);
            else if (IsWindowVisible(msg->hwnd))
                WriteWatermark(msg->hwnd);
            break;
        case WM_SIZE:
            if (msg->wParam == SIZE_MINIMIZED || msg->wParam == SIZE_MAXIMIZED)
                if (IsWindowVisible(msg->hwnd))
                    WriteWatermark(msg->hwnd);
            break;
        case WM_EXITSIZEMOVE:
            if (IsWindowVisible(msg->hwnd))
                WriteWatermark(msg->hwnd);
            break;
        default:
            if (IsWindowVisible(msg->hwnd))
                WriteWatermark(msg->hwnd);
            break;
        }
    }

    return CallNextHookEx(g_wndProcHook, nCode, wParam, lParam);
}


LRESULT CALLBACK WndProcEpilog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPRETSTRUCT    *msg = (CWPRETSTRUCT *)lParam;

    if (IsTopLevelWindow(msg->hwnd) == TRUE)
    {
        switch (msg->message)
        {
        case WM_NCPAINT:
        case WM_PAINT:
            if (IsWindowVisible(msg->hwnd) == TRUE)
//                WriteWatermark(msg->hwnd);
            break;
        default:
            if (IsWindowVisible(msg->hwnd))
//                WriteWatermark(msg->hwnd);
            break;
        }
    }

    return CallNextHookEx(g_wndProcRetHook, nCode, wParam, lParam);
}

LRESULT CALLBACK ShellProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case HSHELL_REDRAW:
    case HSHELL_WINDOWACTIVATED:
    case HSHELL_WINDOWREPLACED:
    case HSHELL_WINDOWCREATED:
        if (IsTopLevelWindow((HWND)wParam) == TRUE && IsWindowVisible((HWND)wParam) == TRUE)
            WriteWatermark((HWND)wParam);
        break;
    case HSHELL_WINDOWDESTROYED:
        if (IsTopLevelWindow((HWND)wParam) == TRUE)
            DwmaxxRemoveWindow((HWND)wParam);
        break;
    }

    return CallNextHookEx(g_shellHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case HCBT_ACTIVATE:
    case HCBT_CREATEWND:
    case HCBT_SETFOCUS:
        if (IsTopLevelWindow((HWND)wParam) == TRUE && IsWindowVisible((HWND)wParam) == TRUE)
            WriteWatermark((HWND)wParam);
        break;
    }

    return CallNextHookEx(g_cbtHook, nCode, wParam, lParam);
}
