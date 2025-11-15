#ifndef VISUALALGORITHMCORE_LIBRARY_H
#define VISUALALGORITHMCORE_LIBRARY_H

#include "FileRead.h"

class MTLFileReadStream final : public FileReadStream
{
  private:
    void initialize() override;

  public:
    void loadFromFile(std::string fileName, Model* const modelPtr) override;
    ~MTLFileReadStream() override = default;
};

#endif // VISUALALGORITHMCORE_LIBRARY_H
