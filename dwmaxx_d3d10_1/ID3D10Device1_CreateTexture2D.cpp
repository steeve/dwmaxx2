#pragma once

#include "ID3D10Device1.h"
#include "globals.h"

METHOD_HOOK(HRESULT, ID3D10Device1, CreateTexture2D, const D3D10_TEXTURE2D_DESC *pDesc,
                                                     const D3D10_SUBRESOURCE_DATA *pInitialData,
                                                     ID3D10Texture2D **ppTexture2D)
{
    if ((pDesc->BindFlags & D3D10_BIND_SHADER_RESOURCE) != 0
        && pDesc->CPUAccessFlags == 0
        && pDesc->Usage == D3D10_USAGE_DEFAULT
        && pDesc->Format == DXGI_FORMAT_B8G8R8A8_UNORM
        && pDesc->MiscFlags == 0
        && ppTexture2D != NULL) // are we trying to create a window? most likely?
    {
        D3D10_TEXTURE2D_DESC newDesc = *pDesc;
        newDesc.MiscFlags |= D3D10_RESOURCE_MISC_SHARED;
        if (__ID3D10Device1_CreateTexture2D(This, &newDesc, pInitialData, ppTexture2D) == S_OK)
            return S_OK;
    }
    return __ID3D10Device1_CreateTexture2D(This, pDesc, pInitialData, ppTexture2D);
}
