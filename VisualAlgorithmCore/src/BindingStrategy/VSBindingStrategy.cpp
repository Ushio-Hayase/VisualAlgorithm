//
// Created by UshioHayase on 2025-11-16.
//

#include "BindingStrategy/VSBindingStrategy.h"

void VSBindingStrategy::bind(ID3D11DeviceContext* context, ID3D11DeviceChild* shader, ID3D11InputLayout* layout) const
{
    context->VSSetShader(static_cast<ID3D11VertexShader*>(shader), nullptr, 0);
    context->IASetInputLayout(layout);
}
