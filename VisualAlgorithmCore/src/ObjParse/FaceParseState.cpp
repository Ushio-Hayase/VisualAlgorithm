//
// Created by UshioHayase on 2025-11-15.
//

#include "ObjParse/FaceParseState.h"

#include "Exception.h"
#include "LogMacro.h"
#include "ObjFileRead.h"
#include "Util.h"

void FaceParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    auto current_mesh = context.getCurrentMesh();
    const auto model_ptr = context.getModelPtr();
    auto history = context.getHistory();
    const auto positions = context.getPositions();
    const auto texture_coors = context.getTextureCoors();
    const auto normals = context.getNormals();

    if (current_mesh == nullptr)
    {
        model_ptr->meshes.emplace_back();
        current_mesh = &model_ptr->meshes.back();
        current_mesh->name = "default";
        current_mesh->materialIdx = -1;
        current_mesh->startIndexLoc = 0;
        current_mesh->indexCount = 0;
        LOG_WARN("Face data found before any 'usemtl' directive. Using default material.");
    }

    if (words.size() < 4)
    {
        LOG_ERROR("The line f is not full");
        throw FileError("File reading error");
    }

    auto& final_indices = model_ptr->indices;
    auto& final_vertices = model_ptr->vertices;

    using namespace utils;

    /* 면일 경우 정점 하나씩 파싱,
     * 만약 정점의 개수가 2^22개 이상이면 오류 */
    for (auto iter = words.begin() + 1; iter != words.end(); ++iter)
    {
        std::vector<std::string> vertex = split_string(*iter, '/');

        int64_t v = std::stoi(vertex[0]), vt = -1, vn = -1;

        int64_t key = (v << 42);

        if (vertex.size() > 1 && !vertex[1].empty())
        {
            vt = std::stoi(vertex[1]);
            key |= (vt << 21);
        }
        if (vertex.size() == 3)
        {
            vn = std::stoi(vertex[2]);
            key |= vn;
        }

        if (history.contains(key))
        {
            final_indices.push_back(history[key]);
            continue;
        }

        Vertex new_vertex{};

        new_vertex.position = positions[v - 1];

        if (vt >= 0)
            new_vertex.textureCoor = texture_coors[vt - 1];
        else
            new_vertex.textureCoor = {0.0f, 0.0f};

        if (vn >= 0)
            new_vertex.normal = normals[vn - 1];
        else
        {
            new_vertex.normal = {0.0f, 0.0f, 0.0f};
        }

        final_vertices.emplace_back(new_vertex);

        uint32_t new_idx = final_vertices.size() - 1;
        final_indices.push_back(new_idx);

        history[key] = new_idx;
    }

    current_mesh->indexCount += 3;
}
