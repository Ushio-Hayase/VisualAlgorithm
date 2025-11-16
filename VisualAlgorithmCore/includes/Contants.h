//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_CONTANTS_H
#define VISUALALGORITHMCORE_CONTANTS_H

#include <DirectXMath.h>

struct alignas(16) PerObjectConstants
{
    DirectX::XMFLOAT4X4 wordsMatrix;
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projMatrix;
};

#endif // VISUALALGORITHMCORE_CONTANTS_H
