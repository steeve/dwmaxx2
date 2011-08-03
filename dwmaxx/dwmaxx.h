#pragma once
#ifdef DWMAXX_EXPORTS
#define DWMAXX_DLLSPEC __declspec(dllexport)
#else
#define DWMAXX_DLLSPEC __declspec(dllimport)
#endif

#include <Windows.h>
#include <dwmapi.h>

HWND                    DwmaxxRpcWindow();

extern "C"
{
DWMAXX_DLLSPEC HRESULT  DwmaxxGetWindowSharedHandle(HWND hWnd, HANDLE *sharedHandle);
DWMAXX_DLLSPEC void     DwmaxxGetWindowSharedHandleAsync(HWND hWnd, HWND callbackHwnd);
DWMAXX_DLLSPEC void     DwmaxxRemoveWindow(HWND hWnd);
DWMAXX_DLLSPEC HRESULT DwmaxxGetExtendedWindowMargins(HWND hWnd, MARGINS *margins);

DWMAXX_DLLSPEC HRESULT  DwmaxxLoad();
DWMAXX_DLLSPEC HRESULT  DwmaxxUnload();
DWMAXX_DLLSPEC BOOL     DwmaxxIsLoaded();
}
