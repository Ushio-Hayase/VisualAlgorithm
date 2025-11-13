//
// Created by UshioHayase on 2025-11-13.
//

#include "renderable_model.h"

RenderableModel::RenderableModel(ID3D11Device* device, const Model& cpuModel)
{
    if (!cpuModel.vertices.empty())
        vertexBuffer = std::make_unique<Buffer<Vertex>>(device, cpuModel.vertices);

    if (!cpuModel.indices.empty())
    {
        bool use32BitIndices = cpuModel.vertices.size() > 65535;

        // 정점이 65535개 이하면 16비트로 생성
        if (use32BitIndices)
            indexBuffer = std::make_unique<Buffer<uint32_t>>(device, cpuModel.indices);
        else
        {
            std::vector<uint16_t> indices16;
            indices16.assign(cpuModel.indices.begin(), cpuModel.indices.end());

            indexBuffer = std::make_unique<Buffer<uint16_t>>(device, indices16);
        }
    }

    meshes = cpuModel.meshes;
}

void RenderableModel::render(ID3D11DeviceContext* context) const
{
    // 버퍼가 없으면 그리지 않음
    if (!vertexBuffer || indexBuffer.index() == std::variant_npos)
    {
        return;
    }

    vertexBuffer->bind(context); // 정점 버퍼 바인딩

    // 인덱스 버퍼 타입에 맞게 바인딩
    std::visit([&](const auto& bufferPtr) { bufferPtr->bind(context); }, indexBuffer);

    for (const auto& mesh : meshes)
    {
        // TODO: 나중에 mesh.materialIdx를 사용해서 메쉬에 맞는 셰이더 설정 필요

        context->DrawIndexed(mesh.indexCount, mesh.startIndexLoc, 0);
    }
}
