//
// Created by ushiohayase on 2025-11-05.
//

#include "ObjFileRead.h"

#include "Exception.h"
#include "LogMacro.h"
#include "MtlFileRead.h"
#include "Util.h"

#include "../includes/ObjParse/FaceParseState.h"
#include "../includes/ObjParse/IObjParseState.h"
#include "../includes/ObjParse/MtlLibParseState.h"
#include "../includes/ObjParse/NormalParseState.h"
#include "../includes/ObjParse/TextureParseState.h"
#include "../includes/ObjParse/UseMtlParseState.h"
#include "../includes/ObjParse/VertexParseState.h"

void OBJFileReadStream::loadFromFile(std::string file_name, Model* const model_ptr)
{
    if (!model_ptr)
    {
        LOG_ERROR("model ptr is null!");
        throw NullPointerError();
    }

    this->file_name = file_name;
    fs = std::ifstream{file_name};
    this->model_ptr = model_ptr;

    if (!fs.is_open())
    {
        LOG_ERROR("Can't open obj file, file name is {}", this->file_name);
        throw FileError("can't open file ");
    }

    initialize();

    std::string line;

    while (std::getline(fs, line))
    {
        using namespace utils;

        // 한 줄씩 파싱
        std::vector<std::string> words = split_string(line, ' ');

        auto it = state_map.find(words[0]);

        if (it == state_map.end())
            continue;

        it->second->parseLine(*this, words);
    }

    auto final_indices = this->model_ptr->indices;
    auto final_vertices = this->model_ptr->vertices;

    /* 법선 벡터 계산 (vn이 없을 경우 대비) */
    for (int i = 0; i < final_indices.size(); i += 3)
    {
        Vector3 p1 = final_vertices[i].position;
        Vector3 p2 = final_vertices[i + 1].position;
        Vector3 p3 = final_vertices[i + 2].position;

        Vector3 face_normal = cross(p3 - p1, p2 - p1);

        final_vertices[i].normal += face_normal;
    }

    /* 법선 정규화 */
    for (auto& vertex : final_vertices)
    {
        vertex.normal /= norm(vertex.normal);
    }

    fs.close();
}

void OBJFileReadStream::parseLine(std::vector<std::string>& words) { state_map[words[0]]->parseLine(*this, words); }

void OBJFileReadStream::registerStates()
{
    state_map.emplace("v", std::make_unique<VertexParseState>());
    state_map.emplace("vt", std::make_unique<TextureParseState>());
    state_map.emplace("vn", std::make_unique<NormalParseState>());
    state_map.emplace("f", std::make_unique<FaceParseState>());
    state_map.emplace("usemtl", std::make_unique<UseMtlParseState>());
    state_map.emplace("mtllib", std::make_unique<MtlLibParseState>());
}

void OBJFileReadStream::initialize()
{
    current_mesh = nullptr;
    history.clear();
    material_name_to_idx.clear();

    positions.clear();
    texture_coors.clear();
    normals.clear();

    file_name.clear();
    fs.clear();
}

std::vector<Vector3>& OBJFileReadStream::getPositions() { return positions; }

std::vector<Vector2>& OBJFileReadStream::getTextureCoors() { return texture_coors; }
std::vector<Vector3>& OBJFileReadStream::getNormals() { return normals; }

std::unordered_map<int64_t, uint32_t>& OBJFileReadStream::getHistory() { return history; }

const std::unordered_map<std::string, int>& OBJFileReadStream::getMaterialNameToIdx() const
{
    return material_name_to_idx;
}

Mesh*& OBJFileReadStream::getCurrentMesh() { return current_mesh; }

Model* OBJFileReadStream::getModelPtr() const { return model_ptr; }
