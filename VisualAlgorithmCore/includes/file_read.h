//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_FILE_READ_H
#define VISUALALGORITHMCORE_FILE_READ_H

#include "model.h"

#include <fstream>

class FileReadStream
{
  protected:
    std::ifstream fs;
    std::string fileName;

    virtual void initialize() = 0;

  public:
    FileReadStream() = default;
    virtual ~FileReadStream() = default;
    virtual void loadFromFile(std::string fileName, Model* const modelPtr) = 0;
};

#endif // VISUALALGORITHMCORE_FILE_READ_H
