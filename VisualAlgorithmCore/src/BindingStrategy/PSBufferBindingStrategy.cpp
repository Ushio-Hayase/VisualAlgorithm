//
// Created by UshioHayase on 2025-11-16.
//

#include "BindingStrategy/PSBufferBindingStrategy.h"

void PSConstantBufferBindStrategy::bind(ID3D11DeviceContext* context, ID3D11Buffer* buffer, UINT slot) const
{
    context->PSSetConstantBuffers(slot, 1, &buffer);
}
