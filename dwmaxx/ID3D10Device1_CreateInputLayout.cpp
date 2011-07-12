#pragma once

#include "ID3D10Device1.h"
#include "globals.h"
#include <stdio.h>

METHOD_HOOK(HRESULT, ID3D10Device1, CreateInputLayout, const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs,
                                                       UINT NumElements,
                                                       const void *pShaderBytecodeWithInputSignature,
                                                       SIZE_T BytecodeLength,
                                                       ID3D10InputLayout **ppInputLayout)
{
    HRESULT res = __ID3D10Device1_CreateInputLayout(This, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    
#ifdef _DEBUG
    printf("New Input Layout:\n");
    for (UINT i = 0; i < NumElements; i++)
    {
        D3D10_INPUT_ELEMENT_DESC elem = pInputElementDescs[i];
        printf("  Name=%s, Format=%u, AlignedByteOffset=%u\n", elem.SemanticName, elem.Format, elem.AlignedByteOffset);
    }
    printf("\n");
#endif

    if (SUCCEEDED(res))
        g_inputLayouts[*ppInputLayout] = NumElements;

    return res;
}
