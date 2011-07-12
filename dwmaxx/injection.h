#pragma once
#include <Windows.h>

#define DWM_CLASS_NAME      ("Dwm")
#define LOAD_LIBRARY_EXPORT	("LoadLibraryA")
#define FREE_LIBRARY_EXPORT	("FreeLibrary")

HMODULE	    RemoteGetModuleHandle(DWORD dwPID, LPCTSTR szDllName);
BOOL        DwmaxxIsRunningInsideDWM();
BOOL        DwmaxxIsInjected();
HRESULT	    DwmaxxInject();
HRESULT     DwmaxxRemoteUnload();
