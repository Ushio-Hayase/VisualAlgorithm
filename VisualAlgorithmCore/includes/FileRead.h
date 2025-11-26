//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_FILE_READ_H
#define VISUALALGORITHMCORE_FILE_READ_H

#include "Model.h"

#include <fstream>

class FileReadStream
{
  protected:
    std::ifstream fs;
    std::string file_name;

    virtual void initialize() = 0;

  public:
    FileReadStream() = default;
    virtual ~FileReadStream() = default;
    virtual void loadFromFile(std::string file_name, Model* const model_ptr) = 0;

    std::string getFileName() { return file_name; }
};

#endif // VISUALALGORITHMCORE_FILE_READ_H
