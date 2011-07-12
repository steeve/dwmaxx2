#pragma once

#include <Windows.h>

#define DLL_NAME				"dwmaxx64.dll"
#define DWM_CLASS_NAME			"Dwm"

#ifdef UNICODE
#define LOAD_LIBRARY_EXPORT	"LoadLibraryW"
#else
#define LOAD_LIBRARY_EXPORT	"LoadLibraryA"
#endif // !UNICODE
#define FREE_LIBRARY_EXPORT	"FreeLibrary"
