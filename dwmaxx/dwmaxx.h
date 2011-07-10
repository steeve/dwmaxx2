#ifdef DWMAXX_EXPORTS
#define DWMAXX_DLLSPEC __declspec(dllexport)
#else
#define DWMAXX_DLLSPEC __declspec(dllimport)
#endif

#include <Windows.h>

HWND                    DwmaxxRpcWindow();
extern "C"
{
DWMAXX_DLLSPEC HANDLE    DwmaxxGetWindowSharedHandle(HWND hWnd);
DWMAXX_DLLSPEC void      DwmaxxGetWindowSharedHandleAsync(HWND hWnd, HWND callbackHwnd);
DWMAXX_DLLSPEC void      DwmaxxRemoveWindow(HWND hWnd);
}
