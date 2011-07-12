#include <Windows.h>
#include <dwmapi.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx.h"
#include "dwmaxx_private.h"
#include "injection.h"
#include "win_hooks.h"
#include "watermarking.h"
#include "hooking.h"
#include "rpc_hwnd.h"

#pragma comment (lib, "dwmapi.lib")

HRESULT DwmaxxLoad()
{
    HRESULT hr = E_FAIL;
    if (DwmaxxIsInjected() == TRUE)
        return (E_FAIL);

    hr = DwmaxxInject();
    if (FAILED(hr))
        return (hr);

    // Now restart composition to get a new D3D device
    DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
    DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);

    return (hr);
}

HRESULT DwmaxxUnload()
{
    if (DwmaxxIsLoaded() == FALSE)
        return (E_FAIL);
    HWND hw = DwmaxxRpcWindow();
    SendMessage(DwmaxxRpcWindow(), DWMAXX_UNLOAD, NULL, NULL);
    return (S_OK);
}

BOOL    DwmaxxIsLoaded()
{
    return (DwmaxxIsInjected());
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
