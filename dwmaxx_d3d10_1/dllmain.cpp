#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "constants.h"
#include "globals.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hD3D10 = LoadLibrary(ORIGNAL_D3D10);
        if (g_hD3D10 == NULL)
            return (FALSE);
        g_isDWM = (GetModuleHandle(DWM_MODULE) != NULL);
#ifdef _DEBUG
        //g_isDWM = false;
        if (g_isDWM == true)
        {
            AllocConsole();
            int hCrt, i;
            FILE *hf;
            hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
            hf = _fdopen(hCrt, "w");
            *stdout = *hf;
            i = setvbuf(stdout, NULL, _IONBF, 0); 
        }
#endif
        break;

    case DLL_PROCESS_DETACH:
        if (g_hD3D10 != NULL)
            FreeLibrary(g_hD3D10);
        break;
	}

	return (TRUE);
}
