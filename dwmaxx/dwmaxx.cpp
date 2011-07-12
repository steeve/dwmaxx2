#include <Windows.h>
#include <dwmapi.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx.h"
#include "injection.h"
#include "win_hooks.h"

#pragma comment (lib, "dwmapi.lib")

BOOL DwmaxxLoad()
{
    if (DwmaxxIsInjected() == true)
        return (FALSE);

    if (DwmaxxInject() == false)
        return (FALSE);

    DwmaxxInstallHooks();

    // Now restart composition to get a new D3D device
    DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
    DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);

    return (TRUE);
}

BOOL	DwmaxxUnload2()
{
    //PostMessage(DwmaxxRpcWindow(), DWMAXX_UNLOAD, NULL, NULL);
    return (TRUE);
}

void    DwmaxxInstallHooks()
{
    g_wndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WndProcProlog, g_hInstance, 0);
    g_wndProcRetHook = SetWindowsHookEx(WH_CALLWNDPROCRET, WndProcEpilog, g_hInstance, 0);
    g_shellHook = SetWindowsHookEx(WH_SHELL, ShellProcProlog, g_hInstance, 0);
}

void    DwmaxxRemoveHooks()
{
    UnhookWindowsHookEx(g_wndProcHook);
    UnhookWindowsHookEx(g_wndProcRetHook);
    UnhookWindowsHookEx(g_shellHook);
}

HWND    DwmaxxRpcWindow()
{
    return (FindWindowEx(HWND_MESSAGE, NULL, DWMAXX_RPC_WINDOW_CLASS, NULL));
}

HANDLE  DwmaxxGetWindowSharedHandle(HWND hWnd)
{
    return (HANDLE)SendMessage(DwmaxxRpcWindow(), DWMAXX_GET_SHARED_HANDLE, (WPARAM)hWnd, NULL);
}

void    DwmaxxGetWindowSharedHandleAsync(HWND hWnd, HWND callbackHwnd)
{
    PostMessage(DwmaxxRpcWindow(), DWMAXX_GET_SHARED_HANDLE, (WPARAM)hWnd, (LPARAM)callbackHwnd);
}

void    DwmaxxRemoveWindow(HWND hWnd)
{
    PostMessage(DwmaxxRpcWindow(), DWMAXX_REMOVE_WINDOW, (WPARAM)hWnd, NULL);
}
