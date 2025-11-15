#ifndef VISUALALGORITHMCORE_LIBRARY_H
#define VISUALALGORITHMCORE_LIBRARY_H

#include "FileRead.h"

constexpr int MAX_NUMBER_OF_ELEM_SIZE = (1 << 22);

class MTLFileReadStream final : FileReadStream
{
  private:
    void initialize() override;

  public:
    void loadFromFile(std::string fileName, Model* const modelPtr) override;
    ~MTLFileReadStream() override = default;
};

#endif // VISUALALGORITHMCORE_LIBRARY_H
