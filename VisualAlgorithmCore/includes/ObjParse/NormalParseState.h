//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_NORMALPARSESTATE_H
#define VISUALALGORITHMCORE_NORMALPARSESTATE_H
#include "IObjParseState.h"

class NormalParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream&, const std::vector<std::string>&) const override;
};

#endif // VISUALALGORITHMCORE_NORMALPARSESTATE_H
