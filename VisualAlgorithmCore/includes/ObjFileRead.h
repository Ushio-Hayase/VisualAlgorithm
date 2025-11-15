//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_OBJ_FILE_READ_H
#define VISUALALGORITHMCORE_OBJ_FILE_READ_H

#include "FileRead.h"

#include <unordered_map>

class IObjParseState;

constexpr int MAX_NUMBER_OF_ELEM_SIZE = (1 << 22);

class OBJFileReadStream final : public FileReadStream
{
  private:
    Mesh* currentMesh = nullptr;

    std::unordered_map<int64_t, uint32_t> history;
    std::unordered_map<std::string, int> materialNameToIdx;

    std::vector<Vector3> positions;
    std::vector<Vector2> textureCoors;
    std::vector<Vector3> normals;

    Model* modelPtr = nullptr;

    std::unordered_map<std::string, std::unique_ptr<IObjParseState>> stateMap;

    void parseLine(std::vector<std::string>& words);
    void registerStates();

    void initialize() override;

  public:
    ~OBJFileReadStream() override = default;

    void loadFromFile(std::string fileName, Model* const modelPtr) override;

    std::vector<Vector3>& getPositions();
    std::vector<Vector2>& getTextureCoors();
    std::vector<Vector3>& getNormals();

    std::unordered_map<int64_t, uint32_t>& getHistory();
    const std::unordered_map<std::string, int>& getMaterialNameToIdx() const;

    Mesh*& getCurrentMesh();
    Model* getModelPtr() const;
};

#endif // VISUALALGORITHMCORE_OBJ_FILE_READ_H
