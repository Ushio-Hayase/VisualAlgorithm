//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_VSBINDINGSTRATEGY_H
#define VISUALALGORITHMCORE_VSBINDINGSTRATEGY_H
#include "IBindingStrategy.h"

class VSBindingStrategy : public IShaderBindStrategy
{
  public:
    void bind(ID3D11DeviceContext* context, ID3D11DeviceChild* shader, ID3D11InputLayout* layout) const override;
};

#endif // VISUALALGORITHMCORE_VSBINDINGSTRATEGY_H
