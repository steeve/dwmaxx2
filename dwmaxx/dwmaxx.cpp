#include <Windows.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx.h"

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
