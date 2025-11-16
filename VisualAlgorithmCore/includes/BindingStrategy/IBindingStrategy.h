//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_IBINDINGSTRATEGY_H
#define VISUALALGORITHMCORE_IBINDINGSTRATEGY_H

#include <d3d11.h>

// 셰이더를 바인딩하는 모든 전략이 구현해야 할 인터페이스
class IShaderBindStrategy
{
  public:
    virtual ~IShaderBindStrategy() = default;
    virtual void bind(ID3D11DeviceContext* context, ID3D11DeviceChild* shader, ID3D11InputLayout* layout) const = 0;
};

// 상수 버퍼를 바인딩하는 모든 전략이 구현해야 할 인터페이스
class IConstantBufferBindStrategy
{
  public:
    virtual ~IConstantBufferBindStrategy() = default;
    virtual void bind(ID3D11DeviceContext* context, ID3D11Buffer* buffer, UINT slot) const = 0;
};
#endif // VISUALALGORITHMCORE_IBINDINGSTRATEGY_H
