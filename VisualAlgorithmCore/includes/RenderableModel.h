//
// Created by UshioHayase on 2025-11-13.
//

#ifndef VISUALALGORITHMCORE_RENDERABLE_MODEL_H
#define VISUALALGORITHMCORE_RENDERABLE_MODEL_H

#include <variant>

#include "Buffer.hpp"

class RenderableModel
{
  private:
    std::unique_ptr<Buffer<Vertex>> vertex_buffer;
    std::variant<std::unique_ptr<Buffer<uint16_t>>, std::unique_ptr<Buffer<uint32_t>>> index_buffer;

    std::vector<Mesh> meshes;

  public:
    RenderableModel(ID3D11Device* device, const Model& cpu_model);
    ~RenderableModel() = default;

    void render(ID3D11DeviceContext* context) const;
};

#endif // VISUALALGORITHMCORE_RENDERABLE_MODEL_H
