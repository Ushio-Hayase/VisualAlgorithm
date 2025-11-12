//
// Created by ushiohayase on 2025-11-05.
//

#include "obj_file_read.h"

#include "exception.h"
#include "mtl_file_read.h"
#include "util.h"

#include <unordered_map>

namespace
{
enum class ObjKeyword
{
    Vertex,
    Texture,
    Normal,
    Face,
    Comment,
    Line,
    Object,
    Group,
    UseMtl,
    MtlLib
};

std::unordered_map<std::string, ObjKeyword> objKeywordMap = {
    {"v", ObjKeyword::Vertex},      {"vt", ObjKeyword::Texture},   {"vn", ObjKeyword::Normal}, {"f", ObjKeyword::Face},
    {"#", ObjKeyword::Comment},     {"l", ObjKeyword::Line},       {"o", ObjKeyword::Object},  {"g", ObjKeyword::Group},
    {"usemtl", ObjKeyword::UseMtl}, {"mtllib", ObjKeyword::MtlLib}};
} // namespace

void OBJFileReadStream::read(Model* const modelPtr)
{
    if (!modelPtr)
        throw NullPointerError();

    if (!fs_.is_open())
        throw FileError("can't open file ");

    std::string line;

    std::vector<Vector3> positions;
    std::vector<Vector2> textureCoors;
    std::vector<Vector3> normals;

    auto& finalVertices = modelPtr->vertex;
    auto& finalIndices = modelPtr->indices;
    auto& finalMeshes = modelPtr->meshes;

    Mesh* currentMesh = nullptr;

    std::unordered_map<int64_t, uint32_t> history;
    std::unordered_map<std::string, int> materialNameToIdx;

    while (std::getline(fs_, line))
    {
        using namespace utils;

        // 한 줄씩 파싱
        std::vector<std::string> words = split_string(line, ' ');

        if (objKeywordMap.contains(words[0]))
            continue;

        switch (ObjKeyword& head = objKeywordMap[words[0]])
        {
        case ObjKeyword::Vertex: {
            positions.emplace_back(stov3(words[1], words[2], words[3]));
            if (positions.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of vertices must be less than "
                                "4,194,304");
            break;
        }
        case ObjKeyword::Texture: {
            textureCoors.emplace_back(stov2(words[1], words[2]));
            if (textureCoors.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of texture_coordinate must be less "
                                "than 4,194,304");
            break;
        }
        case ObjKeyword::Normal: {
            normals.emplace_back(stov3(words[1], words[2], words[3]));
            if (normals.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of vertices must be less "
                                "than 4,194,304");
            break;
        }
        case ObjKeyword::Face: {
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
            break;
        }
        case ObjKeyword::Group: {
            break;
        }
        case ObjKeyword::Comment:
            break;
        case ObjKeyword::UseMtl: {
            std::string& mtlFileName = words[1];
            finalMeshes.emplace_back();
            currentMesh = &finalMeshes.back();

            currentMesh->startIndexLoc = finalIndices.size();

            currentMesh->indexCount = 0;

            break;
        }
        case ObjKeyword::MtlLib: {
            std::string& mtlFileName = words[1];

            MTLFileReadStream mtlFile{mtlFileName};
            mtlFile.read(modelPtr);

            break;
        }
        default:
            break;
        }

        /* 법선 벡터 계산 (vn이 없을 경우 대비) */
        for (int i = 0; i < finalIndices.size(); i += 3)
        {
            Vector3 p1 = finalVertices[i].position;
            Vector3 p2 = finalVertices[i + 1].position;
            Vector3 p3 = finalVertices[i + 2].position;

            Vector3 face_normal = cross(p3 - p1, p2 - p1);

            finalVertices[i].normal += face_normal;
        }

        /* 법선 정규화 */
        for (auto& vertex : finalVertices)
        {
            vertex.normal /= norm(vertex.normal);
        }
    }
}
