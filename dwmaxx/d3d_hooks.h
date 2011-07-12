#pragma once
#include <windows.h>
#include <d3d10_1.h>

void    ApplyD3DDeviceCreationHook();
void    RemoveD3DDeviceCreationHook();
void    ApplyD3DHooks(ID3D10Device1 *pDevice);
void    RemoveD3DHooks(ID3D10Device1 *pDevice);
