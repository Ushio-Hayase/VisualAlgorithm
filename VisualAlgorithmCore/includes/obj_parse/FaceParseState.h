//
// Created by UshioHayase on 2025-11-15.
//

#ifndef VISUALALGORITHMCORE_FACEPARESSTATE_H
#define VISUALALGORITHMCORE_FACEPARESSTATE_H
#include "IObjParseState.h"

class FaceParseState : public IObjParseState
{
  public:
    void parseLine(OBJFileReadStream&, const std::vector<std::string>&) const override;
};

#endif // VISUALALGORITHMCORE_FACEPARESSTATE_H
