//
// Created by UshioHayase on 2025-11-16.
//

#include "BindingStrategy/PSBindingStrategy.h"

void PSBindingStrategy::bind(ID3D11DeviceContext* context, ID3D11DeviceChild* shader, ID3D11InputLayout* layout) const
{
    context->PSSetShader(static_cast<ID3D11PixelShader*>(shader), nullptr, 0);
}
