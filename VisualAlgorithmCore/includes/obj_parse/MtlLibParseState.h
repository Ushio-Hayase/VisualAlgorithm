//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_MTLLIBPARSESTATE_H
#define VISUALALGORITHMCORE_MTLLIBPARSESTATE_H
#include "IObjParseState.h"

class MtlLibParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream&, const std::vector<std::string>&) const override;
};

#endif // VISUALALGORITHMCORE_MTLLIBPARSESTATE_H
