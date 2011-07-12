#include <Windows.h>
#include <stdio.h>
#include <dwmapi.h>

#pragma comment (lib, "dwmapi.lib")

#include "..\dwmaxx\dwmaxx.h"
#pragma comment (lib, "..\\x64\\Debug\\dwmaxx64.lib")

int main(int argc, char **argv)
{
    int a = sizeof(FARPROC);

    intptr_t b = (intptr_t)1 + (intptr_t)2;

    DwmaxxLoad();


    return (0);
}
