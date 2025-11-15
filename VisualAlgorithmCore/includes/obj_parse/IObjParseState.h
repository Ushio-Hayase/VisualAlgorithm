//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_OBJPARSESTATE_H
#define VISUALALGORITHMCORE_OBJPARSESTATE_H
#include <string>
#include <vector>

class OBJFileReadStream;

class IObjParseState
{
  public:
    virtual ~IObjParseState() = default;

    virtual void parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const = 0;
};

#endif // VISUALALGORITHMCORE_OBJPARSESTATE_H
