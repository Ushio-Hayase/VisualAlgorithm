//
// Created by Ushio on 2025-11-16.
//

#ifndef VISUALALGORITHM_RENDERER_H
#define VISUALALGORITHM_RENDERER_H

#include "RenderableModel.h"

#include <d3d11.h>
#include <wrl/client.h>

class Renderer
{
  private:
    Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain = nullptr;
    D3D_FEATURE_LEVEL feature_level;

    std::vector<RenderableModel> models;

  public:
    Renderer();
    ~Renderer();
};

#endif // VISUALALGORITHM_RENDERER_H
