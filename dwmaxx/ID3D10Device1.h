#pragma once

#include <windows.h>
#include <d3d10_1.h>
#include "hooking.h"

DEFINE_METHOD_HOOK(void, ID3D10Device1, Draw, 
    UINT VertexCount,
    UINT StartVertexLocation
)

DEFINE_METHOD_HOOK(void, ID3D10Device1, IASetVertexBuffers,
    UINT StartSlot,
    UINT NumBuffers,
    ID3D10Buffer *const *ppVertexBuffers,
    const UINT *pStrides,
    const UINT *pOffsets
)

DEFINE_METHOD_HOOK(HRESULT, ID3D10Device1, CreateInputLayout,
    const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs,
    UINT NumElements,
    const void *pShaderBytecodeWithInputSignature,
    SIZE_T BytecodeLength,
    ID3D10InputLayout **ppInputLayout
)

DEFINE_METHOD_HOOK(HRESULT, ID3D10Device1, CreateTexture2D,
    const D3D10_TEXTURE2D_DESC *pDesc,
    const D3D10_SUBRESOURCE_DATA *pInitialData,
    ID3D10Texture2D **ppTexture2D
)
