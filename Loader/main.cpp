#include <Windows.h>
#include <stdio.h>
#include <dwmapi.h>

#pragma comment (lib, "dwmapi.lib")

#include "..\dwmaxx\dwmaxx.h"

#ifdef _WIN64
#pragma comment (lib, "..\\x64\\Debug\\dwmaxx64.lib")
#else
#pragma comment (lib, "..\\Debug\\dwmaxx32.lib")
#endif

typedef BOOL (__stdcall *ISPROC)(HWND);

int main(int argc, char **argv)
{
    HWND win = NULL;
    do
    {
        win = FindWindowEx(NULL, win, "CabinetWClass", NULL);
    } while (GetWindowTextLength(win) == 0);


    HRESULT hr;
    HANDLE texHandle;
    MARGINS winMargins;

    hr = DwmaxxGetWindowSharedHandle(win, &texHandle);
    hr = DwmaxxGetExtendedWindowMargins(win, &winMargins);

    return (0);
}
