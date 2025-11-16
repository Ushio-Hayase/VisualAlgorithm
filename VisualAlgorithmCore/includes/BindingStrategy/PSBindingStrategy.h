//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_PSBINDINGSTRATEGY_H
#define VISUALALGORITHMCORE_PSBINDINGSTRATEGY_H
#include "IBindingStrategy.h"

class PSBindingStrategy : public IShaderBindStrategy
{
  public:
    void bind(ID3D11DeviceContext* context, ID3D11DeviceChild* shader, ID3D11InputLayout* layout) const override;
};

#endif // VISUALALGORITHMCORE_PSBINDINGSTRATEGY_H
