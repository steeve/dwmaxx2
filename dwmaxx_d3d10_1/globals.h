#pragma once

#include <map>
#include "types.h"

extern HMODULE                              g_hD3D10;
extern bool                                 g_isDWM;
extern std::map<ID3D10InputLayout *, int>   g_inputLayouts;
extern std::map<HWND, HANDLE>               g_sharedHandles;
extern HWND                                 g_rpcHwnd;
extern ID3D10Texture2D                      *g_pixelTexture;
