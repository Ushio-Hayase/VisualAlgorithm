//
// Created by UshioHayase on 2025-11-13.
//

#ifndef VISUALALGORITHMCORE_GPU_BUFFER_HPP
#define VISUALALGORITHMCORE_GPU_BUFFER_HPP

#include "LogMacro.h"
#include "Model.h"

#include <d3d11.h>

#include <vector>

template <typename T> class Buffer
{
  private:
    ID3D11Buffer* buffer;
    UINT stride;
    UINT elemCount;

  public:
    Buffer(ID3D11Device* device, const std::vector<T>& initialData);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    /**
     * @brief 버퍼를 Input-Assembler에 묶습니다.
     *
     * @param context ID3D11DeviceContext 객체
     */
    void bind(ID3D11DeviceContext* context) const;

    UINT getElemCount() const;
};

template <typename T>
Buffer<T>::Buffer(ID3D11Device* device, const std::vector<T>& initialData)
    : stride(sizeof(T)), elemCount(initialData.size())
{
    ASSERT_NE(elemCount, 0); // 데이터가 비어있으면 오륲

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = stride * elemCount;
    desc.Usage = D3D11_USAGE_DEFAULT; // Gpu Read/Write
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    // 타입에 따라 BindFlags 설정
    if constexpr (std::is_same_v<std::decay_t<T>, Vertex>)
    {
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std ::uint32_t> || std::is_same_v<std::decay_t<T>, uint16_t>)
    {
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    }
    else
    {
        static_assert(false, "Unsupported buffer type provided to Buffer<T> class.");
    }

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = initialData.data();

    HRESULT hr = device->CreateBuffer(&desc, &subresourceData, &buffer);
    ASSERT_EQ(SUCCEEDED(hr), true); // 생성 실패시 프로그램 중단
}

template <typename T> void Buffer<T>::bind(ID3D11DeviceContext* context) const
{
    if constexpr (std::is_same_v<std::decay_t<T>, Vertex>)
    {
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, uint64_t>)
    {
        context->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, uint16_t>)
    {
        context->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
    }
}

template <typename T> UINT Buffer<T>::getElemCount() const { return elemCount; }

template <typename T> Buffer<T>::~Buffer() { buffer->Release(); }

#endif // VISUALALGORITHMCORE_GPU_BUFFER_H
