//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_TEXTUREPARSESTATE_H
#define VISUALALGORITHMCORE_TEXTUREPARSESTATE_H
#include "IObjParseState.h"

class TextureParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream&, const std::vector<std::string>&) const override;
};

#endif // VISUALALGORITHMCORE_TEXTUREPARSESTATE_H
