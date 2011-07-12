#include <Windows.h>
#include <DbgHelp.h>
#include <stdio.h>
#include "hooking.h"
#pragma comment (lib, "dbghelp.lib")

FARPROC HookVTableProc(FARPROC *vtableproc, FARPROC newfunc)
{
    FARPROC oldproc = *vtableproc;
    DWORD prev;
	
    VirtualProtect(vtableproc, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &prev);
	*vtableproc = newfunc;
    VirtualProtect(vtableproc, sizeof(DWORD), prev, &prev);
    
    return (oldproc);
}

#define PE_RvaToVa(handle, rva)				((void *)((intptr_t)handle + (intptr_t)rva))
FARPROC IATPatchSub(char *imageName, char *importImageName, char *importName, FARPROC newProc)
{
	HANDLE	hDll = NULL;

	hDll = GetModuleHandle(imageName);

	void *oldProcAddr = NULL;

	IMAGE_NT_HEADERS			*header = NULL;
	IMAGE_DATA_DIRECTORY		*importsDirectoryEntry = NULL;
	IMAGE_IMPORT_DESCRIPTOR		*imageImports = NULL;

	header = ImageNtHeader((void *)hDll);

	// First, unprotect the assembly.
	DWORD	oldProtect = 0;

	importsDirectoryEntry = (IMAGE_DATA_DIRECTORY *)&header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	imageImports = (IMAGE_IMPORT_DESCRIPTOR *)PE_RvaToVa(hDll, importsDirectoryEntry->VirtualAddress);

    while (imageImports->Characteristics != 0)
	{
		char *currentModule = (char *)PE_RvaToVa(hDll, imageImports->Name);

        if (strcmp(currentModule, importImageName) == 0)
		{
			PIMAGE_THUNK_DATA pOriginalThunks = NULL;
			PIMAGE_THUNK_DATA pThunks = NULL;
			pOriginalThunks = (PIMAGE_THUNK_DATA)PE_RvaToVa(hDll, imageImports->OriginalFirstThunk);
			pThunks = (PIMAGE_THUNK_DATA)PE_RvaToVa(hDll, imageImports->FirstThunk);

			for (int i = 0; pOriginalThunks[i].u1.AddressOfData != 0; i++)
			{
				IMAGE_IMPORT_BY_NAME	*name = (IMAGE_IMPORT_BY_NAME *)PE_RvaToVa(hDll, pOriginalThunks[i].u1.AddressOfData);
				char *currentProc = (char *)&(name->Name);

				if (strcmp(currentProc, importName) == 0)
				{
					oldProcAddr = (void *)pThunks[i].u1.Function;

					VirtualProtect((void *)&pThunks[i], sizeof(pThunks[i]), PAGE_READWRITE, &oldProtect);
					pThunks[i].u1.Function = (intptr_t)newProc;
					VirtualProtect((void *)&pThunks[i], sizeof(pThunks[i]), oldProtect, &oldProtect);

					FlushInstructionCache(GetCurrentProcess(), NULL, 0);

					return ((FARPROC)oldProcAddr);
				}
			}
		}
		imageImports++;
	}

	return (NULL);
}
