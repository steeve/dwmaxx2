#include <Windows.h>
#include <dwmapi.h>
#include <Shlwapi.h>
#include "globals.h"
#include "types.h"
#include "dwmapi_hooks.h"
#include "DwmExtendFrameIntoClientArea.h"

void    LoadEasyHook()
{
    char    currentDir[MAX_PATH];
    char    systemDir[MAX_PATH];
    char    fullPath[MAX_PATH];
    GetModuleFileName(g_hInstance, currentDir, sizeof(currentDir));
    PathRemoveFileSpec(currentDir);
#ifdef _WIN64
    sprintf(fullPath, "%s\\EasyHook64.dll", currentDir);
#else
    sprintf(fullPath, "%s\\EasyHook32.dll", currentDir);
#endif

    g_hEasyHook = LoadLibrary(fullPath);
#ifdef _WIN64
    LhInstallHook = (LHINSTALLHOOK_PROC)GetProcAddress(g_hEasyHook, "LhInstallHook");
    LhUninstallHook = (LHUNINSTALLHOOK_PROC)GetProcAddress(g_hEasyHook, "LhUninstallHook");
    LhUninstallAllHooks = (LHUNINSTALLALLHOOKS_PROC)GetProcAddress(g_hEasyHook, "LhUninstallAllHooks");
    LhSetExclusiveACL = (LHSETEXCLUSIVEACL_PROC)GetProcAddress(g_hEasyHook, "LhSetExclusiveACL");
#else
    LhInstallHook = (LHINSTALLHOOK_PROC)GetProcAddress(g_hEasyHook, "_LhInstallHook@16");
    LhUninstallHook = (LHUNINSTALLHOOK_PROC)GetProcAddress(g_hEasyHook, "_LhUninstallHook@4");
    LhUninstallAllHooks = (LHUNINSTALLALLHOOKS_PROC)GetProcAddress(g_hEasyHook, "_LhUninstallAllHooks@0");
    LhSetExclusiveACL = (LHSETEXCLUSIVEACL_PROC)GetProcAddress(g_hEasyHook, "_LhSetExclusiveACL@12");
#endif
}

void    UnloadEasyHook()
{
    LhUninstallAllHooks();
    FreeLibrary(g_hEasyHook);
    g_hEasyHook = NULL;
}

void    dwmapi_InstallHooks()
{
    if (g_hEasyHook == NULL)
        LoadEasyHook();

    if (g_dwmHook == NULL)
        g_dwmHook = new HOOK_TRACE_INFO();

    DWORD allThreads = 0;
    LhInstallHook(DwmExtendFrameIntoClientArea, My_DwmExtendFrameIntoClientArea, NULL, g_dwmHook);
    LhSetExclusiveACL(&allThreads, 0, g_dwmHook);
}

void    dwmapi_RemoveHooks()
{
    if (g_dwmHook != NULL)
    {
        LhUninstallHook(g_dwmHook);
        delete g_dwmHook;
        g_dwmHook = NULL;
    }

    if (g_hEasyHook != NULL)
        UnloadEasyHook();
}
