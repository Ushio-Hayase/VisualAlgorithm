//
// Created by UshioHayase on 2025-11-15.
//

#include "obj_parse/TextureParseState.h"

#include "Exception.h"
#include "ObjFileRead.h"
#include "Util.h"
#include "log_macro.h"

void TextureParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    auto textureCoors = context.getTextureCoors();

    if (words.size() < 3)
    {
        LOG_ERROR("The line vt is not full");
        throw FileError("File reading error");
    }

    textureCoors.emplace_back(utils::stov2(words[1], words[2]));
    if (textureCoors.size() >= MAX_NUMBER_OF_ELEM_SIZE)
        throw FileError("The total number of texture_coordinate must be less "
                        "than 4,194,304");
}
