//
// Created by ushiohayase on 2025-11-05.
//

#include "ObjFileRead.h"

#include "Exception.h"
#include "MtlFileRead.h"
#include "Util.h"
#include "log_macro.h"

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

__forceinline void wordsLengthCheck(const int src, const int criteria, const char* name)
{
    if (src < criteria)
    {
        LOG_WARN("The line {} is not full", name);
        throw FileError("File reading error");
    }
}
} // namespace

void OBJFileReadStream::loadFromFile(std::string fileName, Model* const modelPtr)
{
    if (!modelPtr)
    {
        LOG_ERROR("model ptr is null!");
        throw NullPointerError();
    }
    this->fileName = fileName;
    fs = std::ifstream{fileName};

    if (!fs.is_open())
    {
        LOG_ERROR("Can't open obj file, file name is {}", this->fileName);
        throw FileError("can't open file ");
    }

    std::string line;

    auto& finalVertices = modelPtr->vertices;
    auto& finalIndices = modelPtr->indices;

    while (std::getline(fs, line))
    {
        using namespace utils;

        // 한 줄씩 파싱
        std::vector<std::string> words = split_string(line, ' ');

        if (objKeywordMap.contains(words[0]))
            continue;

        switch (objKeywordMap[words[0]])
        {
        case ObjKeyword::Vertex: {
            wordsLengthCheck(words.size(), 4, "v");
            positions.emplace_back(stov3(words[1], words[2], words[3]));
            if (positions.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of vertices must be less than "
                                "4,194,304");
            break;
        }
        case ObjKeyword::Texture: {
            wordsLengthCheck(words.size(), 4, "vt");
            textureCoors.emplace_back(stov2(words[1], words[2]));
            if (textureCoors.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of texture_coordinate must be less "
                                "than 4,194,304");
            break;
        }
        case ObjKeyword::Normal: {
            wordsLengthCheck(words.size(), 4, "vn");
            normals.emplace_back(stov3(words[1], words[2], words[3]));
            if (normals.size() >= MAX_NUMBER_OF_ELEM_SIZE)
                throw FileError("The total number of vertices must be less "
                                "than 4,194,304");
            break;
        }
        case ObjKeyword::Face: {
            wordsLengthCheck(words.size(), 4, "f");

            parsingFace(words, modelPtr);

            break;
        }
        case ObjKeyword::Group: {
            break;
        }
        case ObjKeyword::Comment:
            break;
        case ObjKeyword::UseMtl: {
            wordsLengthCheck(words.size(), 2, "usemtl");

            parsingUseMtl(words, modelPtr);

            break;
        }
        case ObjKeyword::MtlLib: {
            wordsLengthCheck(words.size(), 2, "mtllib");

            parsingMtlLib(words, modelPtr);

            break;
        }
        default:
            break;
        }
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

    fs.close();
}

void OBJFileReadStream::parsingFace(std::vector<std::string>& words, Model* const modelPtr)
{
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

void OBJFileReadStream::parsingUseMtl(std::vector<std::string>& words, Model* const modelPtr)
{
    auto& finalMeshes = modelPtr->meshes;
    auto& finalIndices = modelPtr->indices;

    std::string& materialName = words[1];

    int materialIdx = -1;
    if (materialNameToIdx.contains(materialName))
    {
        materialIdx = materialNameToIdx[materialName];
    }
    else
    {
        LOG_WARN("Material '{}' not found in .mtl file", materialName);
    }

    finalMeshes.emplace_back();
    currentMesh = &finalMeshes.back();

    currentMesh->name = materialName;
    currentMesh->materialIdx = materialIdx;
    currentMesh->startIndexLoc = finalIndices.size();

    currentMesh->indexCount = 0;
}

void OBJFileReadStream::parsingMtlLib(std::vector<std::string>& words, Model* const modelPtr)
{
    std::string& mtlFileName = words[1];

    int beforeLastIdx = modelPtr->materials.size() - 1;

    MTLFileReadStream mtlFile;
    mtlFile.loadFromFile(fileName, modelPtr);

    int afterLastIdx = modelPtr->materials.size() - 1;

    for (int i = beforeLastIdx + 1; i < afterLastIdx; ++i)
    {
        materialNameToIdx.emplace(modelPtr->materials[i].name, i);
    }
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
