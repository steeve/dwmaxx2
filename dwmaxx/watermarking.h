#pragma once
#include <Windows.h>

#ifdef DWMAXX_EXPORTS
#define DWMAXX_DLLSPEC __declspec(dllexport)
#else
#define DWMAXX_DLLSPEC __declspec(dllimport)
#endif

void                DwmaxxRunWatermarker32();
void                DwmaxxRunWatermarker64();
void                WriteWatermark(HWND hWnd);
LRESULT CALLBACK    WatermarkerWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern "C"
{
DWMAXX_DLLSPEC int  DwmaxxRunWatermarker();
}
