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
        win = FindWindowEx(NULL, win, "Chrome_WidgetWin_0", NULL);
    } while (GetWindowTextLength(win) == 0);


    DwmaxxLoad();

    HANDLE texHandle = DwmaxxGetWindowSharedHandle(win);
    MARGINS winMargins = DwmaxxGetExtendedWindowMargins(win);

    DwmaxxUnload();
    return (0);
}
