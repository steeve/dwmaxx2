#include <Windows.h>
#include "hooking.h"

FARPROC HookVTableProc(FARPROC *vtableproc, FARPROC newfunc)
{
    FARPROC oldproc = *vtableproc;
    DWORD prev;
	
    VirtualProtect(vtableproc, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &prev);
	*vtableproc = newfunc;
    VirtualProtect(vtableproc, sizeof(DWORD), prev, &prev);
    
    return (oldproc);
}
