//
// Created by UshioHayase on 2025-11-16.
//

#include "obj_parse/MtlLibParseState.h"

#include "Exception.h"
#include "MtlFileRead.h"
#include "ObjFileRead.h"
#include "log_macro.h"

void MtlLibParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    if (words.size() < 2)
    {
        LOG_ERROR("The line mtllib is not full");
        throw FileError("File reading error");
    }

    auto modelPtr = context.getModelPtr();
    auto fileName = context.getFileName();
    auto materialNameToIdx = context.getMaterialNameToIdx();

    std::string mtlFileName = words[1];

    int beforeLastIdx = modelPtr->materials.size() - 1;

    MTLFileReadStream mtlFile;
    mtlFile.loadFromFile(fileName, modelPtr);

    int afterLastIdx = modelPtr->materials.size() - 1;

    for (int i = beforeLastIdx + 1; i < afterLastIdx; ++i)
    {
        materialNameToIdx.emplace(modelPtr->materials[i].name, i);
    }
}
