#include <Windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include "injection.h"
#include "hooking.h"
#include "dwmaxx.h"
#include "D3D10CreateDevice1.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

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

bool	DwmaxxIsRunningInsideDWM()
{
	return (GetModuleHandle("dwm.exe") != NULL);
}

bool	DwmaxxIsInjected()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	TCHAR	modulePath[512];

	if (GetModuleFileName(GetModuleHandle(DLL_NAME), modulePath, sizeof(modulePath)) == 0)
		return (false);

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (false);

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (false);

	return (RemoteGetModuleHandle(dwmPid, modulePath) != NULL);
}

bool	DwmaxxInject()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	HANDLE	hDwmProcess = NULL;
	TCHAR	modulePath[512];
	bool	result = false;

	if (GetModuleFileName(GetModuleHandle(DLL_NAME), modulePath, sizeof(modulePath)) == 0)
		return (false);

	unsigned int		dllNameSizeInBytes = (strlen(modulePath) + 1) * sizeof(char);
	void	*LoadLibraryAddress = (void *)GetProcAddress(GetModuleHandle("kernel32.dll"), LOAD_LIBRARY_EXPORT);
	if (LoadLibraryAddress == NULL)
		return (false);

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (false);

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (false);

	if ((hDwmProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwmPid)) == NULL)
		return (false);

	TCHAR	*remoteDllName = (TCHAR *)VirtualAllocEx(hDwmProcess, NULL, dllNameSizeInBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (remoteDllName == NULL)
	{
		CloseHandle(hDwmProcess);
		return (false);
	}

	if (WriteProcessMemory(hDwmProcess, (LPVOID)remoteDllName, (LPCVOID)modulePath, dllNameSizeInBytes, NULL) == TRUE)
	{
		HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryAddress, (LPVOID)remoteDllName, NULL, NULL);
		if (remoteThreadHandle != NULL)
		{
			WaitForSingleObject(remoteThreadHandle, INFINITE);
			result = true;
		}
	}

	VirtualFreeEx(hDwmProcess, remoteDllName, dllNameSizeInBytes, MEM_DECOMMIT);

	HMODULE hRemoteModule = RemoteGetModuleHandle(dwmPid, modulePath);

	FARPROC	remoteDWMaxxStart = (FARPROC)((intptr_t)hRemoteModule + ((intptr_t)DwmaxxThreadStart - (intptr_t)GetModuleHandle(DLL_NAME)));
	HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteDWMaxxStart, NULL, NULL, NULL);
	if (remoteThreadHandle != NULL)
	{
		WaitForSingleObject(remoteThreadHandle, INFINITE);
		result = true;
	}

	CloseHandle(hDwmProcess);

	return (result);
}

bool	DwmaxxUnload()
{
	DWORD	dwmPid = NULL;
	HWND	hDwmWindow = NULL;
	TCHAR	modulePath[512];
	HANDLE	hDwmProcess = NULL;
	HMODULE	hRemoteModule = NULL;

	if (GetModuleFileName(GetModuleHandle(DLL_NAME), modulePath, sizeof(modulePath)) == 0)
		return (false);

	if ((hDwmWindow = FindWindow(DWM_CLASS_NAME, NULL)) == NULL)
		return (false);

	if (GetWindowThreadProcessId(hDwmWindow, &dwmPid) == 0)
		return (false);

	hRemoteModule = RemoteGetModuleHandle(dwmPid, modulePath);

	void	*FreeLibraryAddress = (void *)GetProcAddress(GetModuleHandle("kernel32.dll"), FREE_LIBRARY_EXPORT);
	if (FreeLibraryAddress == NULL)
		return (false);

	if ((hDwmProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwmPid)) == NULL)
		return (false);

	DWORD	threadRetValue = FALSE;
	do
	{
		HANDLE	remoteThreadHandle = CreateRemoteThread(hDwmProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibraryAddress, (LPVOID)hRemoteModule, NULL, NULL);
		if (remoteThreadHandle != NULL)
			WaitForSingleObject(remoteThreadHandle, INFINITE);
		GetExitCodeThread(remoteThreadHandle, &threadRetValue);
	} while (threadRetValue == TRUE);

	CloseHandle(hDwmProcess);

	return (RemoteGetModuleHandle(dwmPid, modulePath) == NULL);
}

void	DwmaxxThreadStart()
{
    if (DwmaxxIsRunningInsideDWM() == true)
    {
        // Start the hooking dance!
        AllocConsole();

            int hCrt, i;
            FILE *hf;
            hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
            hf = _fdopen(hCrt, "w");
            *stdout = *hf;
            i = setvbuf(stdout, NULL, _IONBF, 0); 

        IAT_HOOK("dwmcore.dll", "d3d10_1.dll", D3D10CreateDevice1);
    }
}

#pragma warning( pop )
