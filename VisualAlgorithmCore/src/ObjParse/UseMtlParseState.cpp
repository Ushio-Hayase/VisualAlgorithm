//
// Created by UshioHayase on 2025-11-16.
//

#include "ObjParse/UseMtlParseState.h"

#include "Exception.h"
#include "ObjFileRead.h"
#include "LogMacro.h"

void UseMtlParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    if (words.size() < 2)
    {
        LOG_ERROR("The line usemtl is not full");
        throw FileError("File reading error");
    }

    auto modelPtr = context.getModelPtr();
    auto materialNameToIdx = context.getMaterialNameToIdx();
    auto currentMesh = context.getCurrentMesh();

    auto& finalMeshes = modelPtr->meshes;
    auto& finalIndices = modelPtr->indices;

    std::string materialName = words[1];

    int materialIdx = -1;
    if (materialNameToIdx.contains(materialName))
    {
        materialIdx = materialNameToIdx[materialName];
    }
    else
    {
        LOG_WARN("Material '{}' not found in .mtl file", materialName);
    }

    finalMeshes.emplace_back();
    currentMesh = &finalMeshes.back();

    currentMesh->name = materialName;
    currentMesh->materialIdx = materialIdx;
    currentMesh->startIndexLoc = finalIndices.size();

    currentMesh->indexCount = 0;
}
