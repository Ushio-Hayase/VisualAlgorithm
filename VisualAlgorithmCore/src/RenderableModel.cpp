//
// Created by UshioHayase on 2025-11-13.
//

#include "RenderableModel.h"

RenderableModel::RenderableModel(ID3D11Device* device, const Model& cpu_model)
{
    if (!cpu_model.vertices.empty())
        vertex_buffer = std::make_unique<Buffer<Vertex>>(device, cpu_model.vertices);

    if (!cpu_model.indices.empty())
    {

        // 정점이 65535개 이하면 16비트로 생성
        if (cpu_model.vertices.size() > 65535)
            index_buffer = std::make_unique<Buffer<uint32_t>>(device, cpu_model.indices);
        else
        {
            std::vector<uint16_t> indices16;
            indices16.assign(cpu_model.indices.begin(), cpu_model.indices.end());

            index_buffer = std::make_unique<Buffer<uint16_t>>(device, indices16);
        }
    }

    meshes = cpu_model.meshes;
}

void RenderableModel::render(ID3D11DeviceContext* context) const
{
    // 버퍼가 없으면 그리지 않음
    if (!vertex_buffer || index_buffer.index() == std::variant_npos)
    {
        return;
    }

    vertex_buffer->bind(context); // 정점 버퍼 바인딩

    // 인덱스 버퍼 타입에 맞게 바인딩
    std::visit([&](const auto& buffer_ptr) { buffer_ptr->bind(context); }, index_buffer);

    for (const auto& mesh : meshes)
    {
        // TODO: 나중에 mesh.materialIdx를 사용해서 메쉬에 맞는 셰이더 설정 필요

        context->DrawIndexed(mesh.indexCount, mesh.startIndexLoc, 0);
    }
}
