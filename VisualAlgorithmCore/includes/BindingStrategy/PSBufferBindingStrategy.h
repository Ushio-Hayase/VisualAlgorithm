//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_PSBUFFERBINDINGSTRATEGY_H
#define VISUALALGORITHMCORE_PSBUFFERBINDINGSTRATEGY_H
#include "IBindingStrategy.h"

class PSConstantBufferBindStrategy : public IConstantBufferBindStrategy
{
  public:
    void bind(ID3D11DeviceContext* context, ID3D11Buffer* buffer, UINT slot) const override;
};

#endif // VISUALALGORITHMCORE_PSBUFFERBINDINGSTRATEGY_H
