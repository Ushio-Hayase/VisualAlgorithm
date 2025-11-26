//
// Created by UshioHayase on 2025-11-16.
//

#include "ObjParse/MtlLibParseState.h"

#include "Exception.h"
#include "LogMacro.h"
#include "MtlFileRead.h"
#include "ObjFileRead.h"

void MtlLibParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    if (words.size() < 2)
    {
        LOG_ERROR("The line mtllib is not full");
        throw FileError("File reading error");
    }

    auto model_ptr = context.getModelPtr();
    auto file_name = context.getFileName();
    auto material_name_to_idx = context.getMaterialNameToIdx();

    std::string mtlFileName = words[1];

    int before_last_idx = model_ptr->materials.size() - 1;

    MTLFileReadStream mtl_file;
    mtl_file.loadFromFile(file_name, model_ptr);

    int after_last_idx = model_ptr->materials.size() - 1;

    for (int i = before_last_idx + 1; i < after_last_idx; ++i)
    {
        material_name_to_idx.emplace(model_ptr->materials[i].name, i);
    }
}
