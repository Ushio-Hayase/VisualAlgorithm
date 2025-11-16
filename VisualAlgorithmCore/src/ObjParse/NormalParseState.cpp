//
// Created by UshioHayase on 2025-11-16.
//

#include "ObjParse/NormalParseState.h"

#include "Exception.h"
#include "ObjFileRead.h"
#include "Util.h"
#include "LogMacro.h"

void NormalParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    if (words.size() < 4)
    {
        LOG_ERROR("The line vn is not full");
        throw FileError("File reading error");
    }

    auto normals = context.getNormals();

    normals.emplace_back(utils::stov3(words[1], words[2], words[3]));
    if (normals.size() >= MAX_NUMBER_OF_ELEM_SIZE)
        throw FileError("The total number of vertices must be less "
                        "than 4,194,304");
}
