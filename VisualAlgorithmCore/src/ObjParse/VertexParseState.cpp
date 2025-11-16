//
// Created by UshioHayase on 2025-11-15.
//

#include "ObjParse/VertexParseState.h"

#include "Exception.h"
#include "ObjFileRead.h"
#include "Util.h"
#include "LogMacro.h"

void VertexParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    using namespace utils;

    auto& positions = context.getPositions();

    if (words.size() < 4)
    {
        LOG_ERROR("The line v is not full");
        throw FileError("File reading error");
    }

    positions.emplace_back(stov3(words[1], words[2], words[3]));
    if (positions.size() >= MAX_NUMBER_OF_ELEM_SIZE)
        throw FileError("The total number of vertices must be less than "
                        "4,194,304");
}
