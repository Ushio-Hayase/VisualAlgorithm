//
// Created by UshioHayase on 2025-11-15.
//

#include "ObjParse/TextureParseState.h"

#include "Exception.h"
#include "LogMacro.h"
#include "ObjFileRead.h"
#include "Util.h"

void TextureParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    auto texture_coors = context.getTextureCoors();

    if (words.size() < 3)
    {
        LOG_ERROR("The line vt is not full");
        throw FileError("File reading error");
    }

    texture_coors.emplace_back(utils::stov2(words[1], words[2]));
    if (texture_coors.size() >= MAX_NUMBER_OF_ELEM_SIZE)
        throw FileError("The total number of texture_coordinate must be less "
                        "than 4,194,304");
}
