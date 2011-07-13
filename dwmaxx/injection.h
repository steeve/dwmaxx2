#pragma once
#include <Windows.h>

#pragma once
#ifdef DWMAXX_EXPORTS
#define DWMAXX_DLLSPEC __declspec(dllexport)
#else
#define DWMAXX_DLLSPEC __declspec(dllimport)
#endif

#define DWM_CLASS_NAME      ("Dwm")
#define LOAD_LIBRARY_EXPORT	("LoadLibraryA")
#define FREE_LIBRARY_EXPORT	("FreeLibrary")

HMODULE	    RemoteGetModuleHandle(DWORD dwPID, LPCTSTR szDllName);
BOOL        DwmaxxIsRunningInsideDWM();
BOOL        DwmaxxIsInjected();
HRESULT     DwmaxxRemoteUnload();

extern "C"
{
DWMAXX_DLLSPEC HRESULT	    DwmaxxInject();
}
