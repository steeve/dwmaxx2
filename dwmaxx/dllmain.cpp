#include <windows.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx.h"
#include "injection.h"
#include "dwmapi_hooks.h"
#include "..\\easyhook\\easyhook.h"

#ifdef _WIN64
#pragma comment (lib, "..\\easyhook\\EasyHook64.lib")
#else
#pragma comment (lib, "..\\easyhook\\EasyHook32.lib")
#endif

char    PATH[1024];

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = (HINSTANCE)hModule;
        IsTopLevelWindow = (ISTOPLEVELWINDOW_PROC)GetProcAddress(LoadLibrary("user32"), "IsTopLevelWindow");
        if (DwmaxxIsRunningInsideDWM() == FALSE)
            dwmapi_InstallHooks();
        break;

    case DLL_PROCESS_DETACH:
        if (DwmaxxIsRunningInsideDWM() == FALSE)
            dwmapi_RemoveHooks();
        LhUninstallAllHooks();
        break;
	}

	return (TRUE);
}
