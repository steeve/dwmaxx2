#pragma once
#include <Windows.h>
#include <d3d10_1.h>
#include "types.h"

void ReadPixel(ID3D10Device1 *device, ID3D10Texture2D *pTexture, int x, int y, DWMWindowPixel *pixel);
HWND GetWindowFromTexture(ID3D10Device1 *device, ID3D10Texture2D *pTex, D3D10_TEXTURE2D_DESC *pTexDesc);
