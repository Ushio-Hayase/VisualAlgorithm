//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_VERTEXPARSESTATE_H
#define VISUALALGORITHMCORE_VERTEXPARSESTATE_H

#include "IObjParseState.h"

class VertexParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const override;
};

#endif // VISUALALGORITHMCORE_VERTEXPARSESTATE_H
