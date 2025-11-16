//
// Created by UshioHayase on 2025-11-16.
//

#include "BindingStrategy/VSBufferBindingStrategy.h"

void VSConstantBufferBindStrategy::bind(ID3D11DeviceContext* context, ID3D11Buffer* buffer, UINT slot) const
{
    context->VSSetConstantBuffers(slot, 1, &buffer);
}
