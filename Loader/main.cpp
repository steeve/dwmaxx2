#include <Windows.h>
#include <stdio.h>
#include <dwmapi.h>

#pragma comment (lib, "dwmapi.lib")

#include "..\dwmaxx\dwmaxx.h"
#pragma comment (lib, "..\\x64\\Debug\\dwmaxx64.lib")

int main(int argc, char **argv)
{
    if (DwmaxxIsLoaded() == FALSE)
        DwmaxxLoad();
    DwmaxxUnload();
    return (0);
}
