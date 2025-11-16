//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_CONSTANTBUFFER_HPP
#define VISUALALGORITHMCORE_CONSTANTBUFFER_HPP

#include "LogMacro.h"

#include "BindingStrategy/IBindingStrategy.h"

#include <d3d11.h>

template <typename T> class ConstantBuffer
{
  private:
    ID3D11Buffer* buffer;

  public:
    ConstantBuffer(ID3D11Device* device);
    ~ConstantBuffer();

    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;

    // Map/Unmap을 이용한 데이터 업데이트 함수
    void update(ID3D11DeviceContext* context, const T& data);

    void bind(ID3D11DeviceContext* context, const IConstantBufferBindStrategy& strategy, UINT slot) const;
};

template <typename T> ConstantBuffer<T>::ConstantBuffer(ID3D11Device* device)
{
    // 상수 버퍼의 크기는 반드시 16바이트의 배수여야 함!
    static_assert(sizeof(T) % 16 == 0, "ConstantBuffer data size must be a multiple of 16 bytes.");

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(T);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    HRESULT hr = device->CreateBuffer(&desc, nullptr, &buffer);
    ASSERT(SUCCEEDED(hr));
}

template <typename T> ConstantBuffer<T>::~ConstantBuffer() { delete buffer; }

template <typename T> void ConstantBuffer<T>::update(ID3D11DeviceContext* context, const T& data)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &data, sizeof(T));
    context->Unmap(buffer, 0);
}

template <typename T>
void ConstantBuffer<T>::bind(ID3D11DeviceContext* context, const IConstantBufferBindStrategy& strategy, UINT slot) const
{
    strategy.bind(context, buffer, slot);
}

#endif // VISUALALGORITHMCORE_CONSTANTBUFFER_HPP
