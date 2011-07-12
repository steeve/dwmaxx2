#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <dwmapi.h>
#include "constants.h"
#include "globals.h"
#include "watermarking.h"
#include "win_hooks.h"

#pragma comment (lib, "shlwapi.lib")
#pragma comment (lib, "dwmapi.lib")

void    DwmaxxRunWatermarker32()
{
    char    currentDir[MAX_PATH];
    char    systemDir[MAX_PATH];
    char    fullPath[MAX_PATH];
    GetModuleFileName(g_hInstance, currentDir, sizeof(fullPath));
    PathRemoveFileSpec(currentDir);
#ifdef _WIN64
    GetSystemWow64Directory(systemDir, sizeof(systemDir));
#else
    GetSystemDirectory(systemDir, sizeof(systemDir));
#endif
    sprintf(fullPath, "%s\\rundll32.exe %s\\dwmaxx32.dll,DwmaxxRunWatermarker", systemDir, currentDir);
    PROCESS_INFORMATION pi;
    STARTUPINFO         si;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, fullPath, NULL, NULL, NULL, NULL, NULL, NULL, &si, &g_watermarker32);
}

void    DwmaxxRunWatermarker64()
{
    char    currentDir[MAX_PATH];
    char    systemDir[MAX_PATH];
    char    fullPath[MAX_PATH];
    GetModuleFileName(g_hInstance, currentDir, sizeof(fullPath));
    PathRemoveFileSpec(currentDir);
    GetSystemDirectory(systemDir, sizeof(systemDir));
    sprintf(fullPath, "%s\\rundll32.exe %s\\dwmaxx64.dll,DwmaxxRunWatermarker", systemDir, currentDir);
    PROCESS_INFORMATION pi;
    STARTUPINFO         si;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, fullPath, NULL, NULL, NULL, NULL, NULL, NULL, &si, &g_watermarker64);
}

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

LRESULT CALLBACK WatermarkerWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DWMCOMPOSITIONCHANGED:
        {
            BOOL compositionEnabled = FALSE;
            DwmIsCompositionEnabled(&compositionEnabled);
            if (compositionEnabled == TRUE)
                InvalidateRect(NULL, NULL, TRUE);
        }
        break;
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}


int    DwmaxxRunWatermarker()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = (WNDPROC)WatermarkerWindowProc;
    wc.lpszClassName = DWMAXX_WATERMARKER_CLASS;
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(NULL, DWMAXX_WATERMARKER_CLASS, NULL, NULL, 0, 0, 1, 1, HWND_MESSAGE, NULL, (HINSTANCE)GetModuleHandle(NULL), NULL);
    
    DwmaxxInstallHooks();

    InvalidateRect(NULL, NULL, TRUE);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
        DispatchMessage(&msg);

    DwmaxxRemoveHooks();

    return ((int)msg.wParam);
}
