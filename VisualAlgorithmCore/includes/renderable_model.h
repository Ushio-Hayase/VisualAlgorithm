//
// Created by UshioHayase on 2025-11-13.
//

#ifndef VISUALALGORITHMCORE_RENDERABLE_MODEL_H
#define VISUALALGORITHMCORE_RENDERABLE_MODEL_H

#include <variant>

#include "buffer.hpp"

class RenderableModel
{
  private:
    std::unique_ptr<Buffer<Vertex>> vertexBuffer;
    std::variant<std::unique_ptr<Buffer<uint16_t>>, std::unique_ptr<Buffer<uint32_t>>> indexBuffer;

    std::vector<Mesh> meshes;

  public:
    RenderableModel(ID3D11Device* device, const Model& cpuModel);
    ~RenderableModel() = default;

    void render(ID3D11DeviceContext* context) const;
};

#endif // VISUALALGORITHMCORE_RENDERABLE_MODEL_H
