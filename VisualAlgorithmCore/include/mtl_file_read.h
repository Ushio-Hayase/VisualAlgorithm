#ifndef VISUALALGORITHMCORE_LIBRARY_H
#define VISUALALGORITHMCORE_LIBRARY_H

#include "file_read.h"

constexpr int MAX_NUMBER_OF_ELEM_SIZE = (1 << 22);

class MTLFileReadStream final : FileReadStream
{
   public:
    MTLFileReadStream(std::string fileName) : FileReadStream(fileName) {}
    void read(Model* const modelPtr) override;
    virtual ~MTLFileReadStream() = default;
};

#endif  // VISUALALGORITHMCORE_LIBRARY_H
