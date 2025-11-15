//
// Created by UshioHayase on 2025-11-15.
//

#include "obj_parse/FaceParseState.h"

#include "Exception.h"
#include "ObjFileRead.h"
#include "Util.h"
#include "log_macro.h"

void FaceParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    auto currentMesh = context.getCurrentMesh();
    const auto modelPtr = context.getModelPtr();
    auto history = context.getHistory();
    const auto positions = context.getPositions();
    const auto textureCoors = context.getTextureCoors();
    const auto normals = context.getNormals();

    if (currentMesh == nullptr)
    {
        modelPtr->meshes.emplace_back();
        currentMesh = &modelPtr->meshes.back();
        currentMesh->name = "default";
        currentMesh->materialIdx = -1;
        currentMesh->startIndexLoc = 0;
        currentMesh->indexCount = 0;
        LOG_WARN("Face data found before any 'usemtl' directive. Using default material.");
    }

    if (words.size() < 4)
    {
        LOG_ERROR("The line f is not full");
        throw FileError("File reading error");
    }

    auto& finalIndices = modelPtr->indices;
    auto& finalVertices = modelPtr->vertices;

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
            finalIndices.push_back(history[key]);
            continue;
        }

        Vertex newVertex{};

        newVertex.position = positions[v - 1];

        if (vt >= 0)
            newVertex.textureCoor = textureCoors[vt - 1];
        else
            newVertex.textureCoor = {0.0f, 0.0f};

        if (vn >= 0)
            newVertex.normal = normals[vn - 1];
        else
        {
            newVertex.normal = {0.0f, 0.0f, 0.0f};
        }

        finalVertices.emplace_back(newVertex);

        uint32_t new_idx = finalVertices.size() - 1;
        finalIndices.push_back(new_idx);

        history[key] = new_idx;
    }

    currentMesh->indexCount += 3;
}
