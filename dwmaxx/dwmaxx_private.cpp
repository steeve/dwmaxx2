#include <Windows.h>
#include <dwmapi.h>
#include <Shlwapi.h>
#include "globals.h"
#include "constants.h"
#include "dwmaxx_private.h"
#include "injection.h"
#include "win_hooks.h"
#include "watermarking.h"
#include "hooking.h"
#include "rpc_hwnd.h"
#include "d3d_hooks.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#pragma comment (lib, "dwmapi.lib")

void	DwmaxxThreadStart()
{
    if (DwmaxxIsRunningInsideDWM() == TRUE)
    {
        // Start the hooking dance!
#ifdef _DEBUG
        AllocConsole();
        int hCrt, i;
        FILE *hf;
        hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        hf = _fdopen(hCrt, "w");
        *stdout = *hf;
        i = setvbuf(stdout, NULL, _IONBF, 0); 
#endif

        if (g_rpcHwnd == NULL)
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateRpcWindow, NULL, 0, NULL);

        ApplyD3DDeviceCreationHook();

        DwmaxxRunWatermarker32();
#ifdef _WIN64
        DwmaxxRunWatermarker64();
#endif
    }
}

HRESULT     DwmaxxTerminate()
{
    if (DwmaxxIsRunningInsideDWM() == TRUE)
    {
#ifdef _DEBUG
        FreeConsole();
#endif
        if (g_watermarker32.hProcess != NULL)
        {
            DWORD   exitCode;
            GetExitCodeProcess(g_watermarker32.hProcess, &exitCode);
            TerminateProcess(g_watermarker32.hProcess, exitCode);
        }
#ifdef _WIN64
        if (g_watermarker64.hProcess != NULL)
        {
            DWORD   exitCode;
            GetExitCodeProcess(g_watermarker64.hProcess, &exitCode);
            TerminateProcess(g_watermarker64.hProcess, exitCode);
        }
#endif

        DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);

        RemoveD3DDeviceCreationHook();

        DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);

        FreeLibraryAndExitThread(g_hInstance, 0);
        return (S_OK);
    }
    return (E_FAIL);
}

HWND    DwmaxxRpcWindow()
{
    return (FindWindowEx(HWND_MESSAGE, NULL, DWMAXX_RPC_WINDOW_CLASS, NULL));
}
