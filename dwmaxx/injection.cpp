#include <Windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include "injection.h"
#include "hooking.h"
#include "dwmaxx.h"
#include "dwmaxx_private.h"
#include "D3D10CreateDevice1.h"
#include "watermarking.h"
#include "globals.h"

#pragma warning( push )
#pragma warning( disable : 4311 4312 )

HMODULE	RemoteGetModuleHandle(DWORD dwPID, LPCTSTR szDllName)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return (NULL);

	// Set the size of the structure before using it.
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (Module32First(hModuleSnap, &me32) == FALSE)
	{
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return (NULL);
	}

	// Now walk the module list of the process,
	// and display information about each module
	do
	{
		if (strcmp(szDllName, me32.szExePath) == 0)
			return ((HMODULE)me32.modBaseAddr);
	} while (Module32Next(hModuleSnap, &me32) == TRUE);

	CloseHandle(hModuleSnap);

	return (NULL);
}

BOOL    DwmaxxIsRunningInsideDWM()
{
    return (GetModuleHandle("dwm.exe") == GetModuleHandle(NULL));
}

BOOL    DwmaxxIsInjected()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	TCHAR	modulePath[512];

	if (GetModuleFileName(g_hInstance, modulePath, sizeof(modulePath)) == 0)
		return (false);

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (false);

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (false);

	return (RemoteGetModuleHandle(dwmPid, modulePath) != NULL);
}

HRESULT	DwmaxxInject()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	HANDLE	hDwmProcess = NULL;
	TCHAR	modulePath[512];
	bool	result = false;

	if (GetModuleFileName(g_hInstance, modulePath, sizeof(modulePath)) == 0)
        return (HRESULT_FROM_WIN32(GetLastError()));

	unsigned int		dllNameSizeInBytes = strlen(modulePath);
	void	*LoadLibraryAddress = (void *)GetProcAddress(GetModuleHandle("kernel32.dll"), LOAD_LIBRARY_EXPORT);
	if (LoadLibraryAddress == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if ((hDwmProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwmPid)) == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	char    *remoteDllName = (char *)VirtualAllocEx(hDwmProcess, NULL, dllNameSizeInBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (remoteDllName == NULL)
	{
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(hDwmProcess);
		return (hr);
	}

	if (WriteProcessMemory(hDwmProcess, (LPVOID)remoteDllName, (LPCVOID)modulePath, dllNameSizeInBytes, NULL) == TRUE)
	{
		HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryAddress, (LPVOID)remoteDllName, NULL, NULL);
		if (remoteThreadHandle != NULL)
		{
			WaitForSingleObject(remoteThreadHandle, INFINITE);
			result = S_OK;
		}
        else
            return (HRESULT_FROM_WIN32(GetLastError()));
	}
    else
        return (HRESULT_FROM_WIN32(GetLastError()));

	VirtualFreeEx(hDwmProcess, remoteDllName, dllNameSizeInBytes, MEM_DECOMMIT);

	HMODULE hRemoteModule = RemoteGetModuleHandle(dwmPid, modulePath);

	FARPROC	remoteDWMaxxStart = (FARPROC)((intptr_t)hRemoteModule + ((intptr_t)DwmaxxThreadStart - (intptr_t)g_hInstance));
	HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteDWMaxxStart, NULL, NULL, NULL);
	if (remoteThreadHandle != NULL)
	{
		WaitForSingleObject(remoteThreadHandle, INFINITE);
		result = S_OK;
	}
    else
        return (HRESULT_FROM_WIN32(GetLastError()));

	CloseHandle(hDwmProcess);

	return (result);
}

HRESULT DwmaxxRemoteUnload()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	TCHAR	modulePath[512];
	HANDLE	hDwmProcess = NULL;
	HMODULE	hRemoteModule = NULL;

	if (GetModuleFileName(g_hInstance, modulePath, sizeof(modulePath)) == 0)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (HRESULT_FROM_WIN32(GetLastError()));

	hRemoteModule = RemoteGetModuleHandle(dwmPid, modulePath);

	void	*FreeLibraryAddress = (void *)GetProcAddress(GetModuleHandle("kernel32.dll"), FREE_LIBRARY_EXPORT);
	if (FreeLibraryAddress == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	if ((hDwmProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwmPid)) == NULL)
		return (HRESULT_FROM_WIN32(GetLastError()));

	DWORD	threadRetValue = FALSE;
	do
	{
		HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibraryAddress, (LPVOID)hRemoteModule, NULL, NULL);
		if (remoteThreadHandle != NULL)
			WaitForSingleObject(remoteThreadHandle, INFINITE);
		GetExitCodeThread(remoteThreadHandle, &threadRetValue);
	} while (threadRetValue == TRUE);

	CloseHandle(hDwmProcess);

    if (RemoteGetModuleHandle(dwmPid, modulePath) == NULL)
        return (S_OK);
    return (E_FAIL);
}

#pragma warning( pop )
