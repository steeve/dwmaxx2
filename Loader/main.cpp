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

int main(int argc, char **argv)
{
    if (DwmaxxIsLoaded() == FALSE)
        DwmaxxLoad();
    DwmaxxUnload();
    return (0);
}
