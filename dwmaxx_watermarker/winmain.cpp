#include <Windows.h>
#include <dwmapi.h>
#include "..\\dwmaxx\\dwmaxx.h"

#pragma comment (lib, "dwmapi.lib")

#ifdef _WIN64
    #define DWMAXX_WATERMARKER_CLASS     ("DwmaxxWatermarker64")
    #ifdef _DEBUG
        #pragma comment(lib, "..\\x64\\Debug\\dwmaxx64.lib")
    #else
        #pragma comment(lib, "..\\x64\\Release\\dwmaxx64.lib")
    #endif
#else
    #define DWMAXX_WATERMARKER_CLASS     ("DwmaxxWatermarker32")
    #ifdef _DEBUG
        #pragma comment(lib, "..\\Debug\\dwmaxx32.lib")
    #else
        #pragma comment(lib, "..\\Release\\dwmaxx32.lib")
    #endif
#endif

HWND    g_hWnd = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.lpszClassName = DWMAXX_WATERMARKER_CLASS;
    RegisterClass(&wc);

    g_hWnd = CreateWindowEx(NULL, DWMAXX_WATERMARKER_CLASS, NULL, NULL, 0, 0, 1, 1, HWND_MESSAGE, NULL, hInstance, NULL);
    
    DwmaxxInstallHooks();

    InvalidateRect(NULL, NULL, TRUE);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
        DispatchMessage(&msg);

    DwmaxxRemoveHooks();

    return ((int)msg.wParam);
}
