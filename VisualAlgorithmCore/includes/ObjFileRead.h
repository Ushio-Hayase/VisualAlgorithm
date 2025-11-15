//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_OBJ_FILE_READ_H
#define VISUALALGORITHMCORE_OBJ_FILE_READ_H

#include "FileRead.h"

#include <unordered_map>

class OBJFileReadStream final : FileReadStream
{
  private:
    Mesh* currentMesh = nullptr;

    std::unordered_map<int64_t, uint32_t> history;
    std::unordered_map<std::string, int> materialNameToIdx;

    std::vector<Vector3> positions;
    std::vector<Vector2> textureCoors;
    std::vector<Vector3> normals;

    void parsingFace(std::vector<std::string>& words, Model* const modelPtr);
    void parsingUseMtl(std::vector<std::string>& words, Model* const modelPtr);
    void parsingMtlLib(std::vector<std::string>& words, Model* const modelPtr);

    void initialize() override;

  public:
    void loadFromFile(std::string fileName, Model* const modelPtr) override;
    ~OBJFileReadStream() override = default;
};

#endif // VISUALALGORITHMCORE_OBJ_FILE_READ_H
