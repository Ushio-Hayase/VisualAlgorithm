//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_VSBUFFERBINDINGSTRATEGY_H
#define VISUALALGORITHMCORE_VSBUFFERBINDINGSTRATEGY_H
#include "IBindingStrategy.h"

class VSConstantBufferBindStrategy : public IConstantBufferBindStrategy
{
  public:
    void bind(ID3D11DeviceContext* context, ID3D11Buffer* buffer, UINT slot) const override;
};

#endif // VISUALALGORITHMCORE_VSBUFFERBINDINGSTRATEGY_H
