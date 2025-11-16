//
// Created by ushiohayase on 2025-11-05.
//

#include "ObjFileRead.h"

#include "Exception.h"
#include "MtlFileRead.h"
#include "Util.h"
#include "LogMacro.h"

#include "../includes/ObjParse/FaceParseState.h"
#include "../includes/ObjParse/IObjParseState.h"
#include "../includes/ObjParse/MtlLibParseState.h"
#include "../includes/ObjParse/NormalParseState.h"
#include "../includes/ObjParse/TextureParseState.h"
#include "../includes/ObjParse/UseMtlParseState.h"
#include "../includes/ObjParse/VertexParseState.h"

void OBJFileReadStream::loadFromFile(std::string fileName, Model* const modelPtr)
{
    if (!modelPtr)
    {
        LOG_ERROR("model ptr is null!");
        throw NullPointerError();
    }

    this->fileName = fileName;
    fs = std::ifstream{fileName};
    this->modelPtr = modelPtr;

    if (!fs.is_open())
    {
        LOG_ERROR("Can't open obj file, file name is {}", this->fileName);
        throw FileError("can't open file ");
    }

    std::string line;

    while (std::getline(fs, line))
    {
        using namespace utils;

        // 한 줄씩 파싱
        std::vector<std::string> words = split_string(line, ' ');

        auto it = stateMap.find(words[0]);

        if (it == stateMap.end())
            continue;

        it->second->parseLine(*this, words);
    }

    auto finalIndices = this->modelPtr->indices;
    auto finalVertices = this->modelPtr->vertices;

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

    fs.close();
}

void OBJFileReadStream::parseLine(std::vector<std::string>& words)
{
    stateMap[words[0]]->parseLine(*this, words);
}

void OBJFileReadStream::registerStates()
{
    stateMap.emplace("v", std::make_unique<VertexParseState>());
    stateMap.emplace("vt", std::make_unique<TextureParseState>());
    stateMap.emplace("vn", std::make_unique<NormalParseState>());
    stateMap.emplace("f", std::make_unique<FaceParseState>());
    stateMap.emplace("usemtl", std::make_unique<UseMtlParseState>());
    stateMap.emplace("mtllib", std::make_unique<MtlLibParseState>());
}

void OBJFileReadStream::initialize()
{
    currentMesh = nullptr;
    history.clear();
    materialNameToIdx.clear();

    positions.clear();
    textureCoors.clear();
    normals.clear();

    fileName.clear();
    fs.clear();
}

std::vector<Vector3>& OBJFileReadStream::getPositions()
{
    return positions;
}

std::vector<Vector2>& OBJFileReadStream::getTextureCoors()
{
    return textureCoors;
}
std::vector<Vector3>& OBJFileReadStream::getNormals()
{
    return normals;
}

std::unordered_map<int64_t, uint32_t>& OBJFileReadStream::getHistory()
{
    return history;
}

const std::unordered_map<std::string, int>& OBJFileReadStream::getMaterialNameToIdx() const
{
    return materialNameToIdx;
}

Mesh*& OBJFileReadStream::getCurrentMesh()
{
    return currentMesh;
}

Model* OBJFileReadStream::getModelPtr() const
{
    return modelPtr;
}
