#include <windows.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx.h"
#include "injection.h"
#include "dwmapi_hooks.h"


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
        break;
	}

	return (TRUE);
}
