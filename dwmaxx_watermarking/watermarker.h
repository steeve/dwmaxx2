#ifdef DWMAXX_WATERMARKER_EXPORTS
#define DWMAXX_WATERMARKER_DLLSPEC __declspec(dllexport)
#else
#define DWMAXX_WATERMARKER_DLLSPEC __declspec(dllimport)
#endif

void                                WriteWatermark(HWND hWnd);
DWMAXX_WATERMARKER_DLLSPEC void     DwmaxxInstallHooks();
DWMAXX_WATERMARKER_DLLSPEC void     DwmaxxRemoveHooks();
