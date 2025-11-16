//
// Created by UshioHayase on 2025-11-16.
//

#ifndef VISUALALGORITHMCORE_SHADER_HPP
#define VISUALALGORITHMCORE_SHADER_HPP

#include "Exception.h"
#include "LogMacro.h"

#include "BindingStrategy/IBindingStrategy.h"
#include "BindingStrategy/PSBindingStrategy.h"
#include "BindingStrategy/VSBindingStrategy.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>
#include <fstream>
#include <vector>

template <typename TShaderType> class Shader
{
  private:
    TShaderType* shader = nullptr;
    ID3D11InputLayout* inputLayout = nullptr;
    std::vector<char> byteCode;

    std::unique_ptr<IShaderBindStrategy> strategy;

  public:
    Shader(ID3D11Device* device, const std::string& filePath);
    ~Shader();

    void bind(ID3D11DeviceContext* context) const;

    ID3D11InputLayout* getInputLayout() const { return inputLayout; }
    const std::vector<char>& getByteCode() const { return byteCode; }
};

template <typename TShaderType> Shader<TShaderType>::Shader(ID3D11Device* device, const std::string& filePath)
{
    
    std::ifstream file{filePath, std::ios::in | std::ios::binary};

    if (!file.is_open())
    {
        LOG_ERROR("Can't open shader file, file path is {}", filePath);
        throw FileError("Can't open shader file");
    }

    file.seekg(0, std::ios::end);
    
    size_t fileSize = file.tellg();

    byteCode.reserve(fileSize);
    
    file.read(byteCode.data(), fileSize);

    file.close();

    D3DCompileFromFile(filePath, );
    
    HRESULT hr;

    if constexpr (std::is_same_v<std::decay_t<TShaderType>, ID3D11VertexShader>)
    {
        hr = device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &shader);

        ASSERT(SUCCEEDED(hr));

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            // Semantic Name, Semantic Index, Format, Input Slot, Aligned Byte Offset, ...

            // 1번 줄: "POSITION" 시맨틱에 대해
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

            // 2번 줄: "TEXCOORD" 시맨틱에 대해 (인덱스 0)
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            // D3D11_APPEND_ALIGNED_ELEMENT 매크로를 사용하면 오프셋(12)을 자동으로 계산해줌

            // 3번 줄: "NORMAL" 시맨틱에 대해
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}};

        hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), byteCode.data(), byteCode.size(), &inputLayout);

        ASSERT(SUCCEEDED(hr));

        strategy = std::make_unique<VSBindingStrategy>();
    }
    else if constexpr (std::is_same_v<TShaderType, ID3D11PixelShader>)
    {
        hr = device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &shader);
        ASSERT(SUCCEEDED(hr));

        strategy = std::make_unique<PSBindingStrategy>();
    }
    else
    {
        static_assert(false, "Unsupported shader type provided to Shader<T> class.");
    }

}

template <typename TShaderType> Shader<TShaderType>::~Shader()
{
    delete shader;
    delete inputLayout;
}

template <typename TShaderType> void Shader<TShaderType>::bind(ID3D11DeviceContext* context) const
{
    strategy->bind(context, shader, inputLayout);
}

using VertexShader = Shader<ID3D11VertexShader>;
using PixelShader = Shader<ID3D11PixelShader>;

#endif // VISUALALGORITHMCORE_SHADER_HPP
