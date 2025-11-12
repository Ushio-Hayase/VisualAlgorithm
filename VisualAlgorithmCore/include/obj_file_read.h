//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_OBJ_FILE_READ_H
#define VISUALALGORITHMCORE_OBJ_FILE_READ_H

#include "file_read.h"

class OBJFileReadStream final : FileReadStream
{
   public:
    OBJFileReadStream(std::string fileName) : FileReadStream(fileName) {}
    void read(Model* const modelPtr) override;
    ~OBJFileReadStream() = default;
};

#endif  // VISUALALGORITHMCORE_OBJ_FILE_READ_H
