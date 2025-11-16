//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_USEMTLPARSESTATE_H
#define VISUALALGORITHMCORE_USEMTLPARSESTATE_H
#include "IObjParseState.h"

class UseMtlParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream&, const std::vector<std::string>&) const override;
};

#endif // VISUALALGORITHMCORE_USEMTLPARSESTATE_H
