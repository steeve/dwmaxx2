#include <map>
#include "types.h"

HMODULE                             g_hD3D10 = NULL;
bool                                g_isDWM = false;
std::map<ID3D10InputLayout *, int>  g_inputLayouts;
std::map<HWND, HANDLE>              g_sharedHandles;
HWND                                g_rpcHwnd = NULL;
ID3D10Texture2D                     *g_pixelTexture = NULL;
