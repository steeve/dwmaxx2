#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "globals.h"

#ifdef _WIN64
    #ifdef _DEBUG
        #pragma comment(lib, "..\\x64\\Debug\\dwmaxx64.lib")
    #else
        #pragma comment(lib, "..\\x64\\Release\\dwmaxx64.lib")
    #endif
#else
    #ifdef _DEBUG
        #pragma comment(lib, "..\\Debug\\dwmaxx32.lib")
    #else
        #pragma comment(lib, "..\\Release\\dwmaxx32.lib")
    #endif
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = (HINSTANCE)hModule;
        break;

    case DLL_PROCESS_DETACH:
        break;
	}

	return (TRUE);
}
