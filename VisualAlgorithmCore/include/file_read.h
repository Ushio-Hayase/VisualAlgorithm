#ifndef VISUALALGORITHMCORE_LIBRARY_H
#define VISUALALGORITHMCORE_LIBRARY_H

#include <fstream>

#include "model.h"

constexpr int MAX_NUMBER_OF_VERTEX_SIZE = (1 << 22);

class FileReadStream
{
   protected:
    std::ifstream fs_;

   public:
    virtual ~FileReadStream() = default;
    virtual void read(std::string& file_name, Model* out_model) = 0;
};

class OBJFileReadStream : FileReadStream
{
   public:
    void read(std::string& file_name, Model* out_model) final override;
};

#endif  // VISUALALGORITHMCORE_LIBRARY_H
