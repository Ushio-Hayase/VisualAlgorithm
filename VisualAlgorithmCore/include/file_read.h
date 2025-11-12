//
// Created by ushiohayase on 2025-11-05.
//

#ifndef VISUALALGORITHMCORE_FILE_READ_H
#define VISUALALGORITHMCORE_FILE_READ_H

#include <fstream>

#include "model.h"

class FileReadStream
{
   protected:
    std::ifstream fs_;

   public:
    FileReadStream(std::string fileName) : fs_(fileName) {}
    virtual ~FileReadStream() = default;
    virtual void read(Model* const modelPtr) = 0;
};

#endif  // VISUALALGORITHMCORE_FILE_READ_H
