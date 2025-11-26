//
// Created by UshioHayase on 2025-11-16.
//

#include "ObjParse/UseMtlParseState.h"

#include "Exception.h"
#include "LogMacro.h"
#include "ObjFileRead.h"

void UseMtlParseState::parseLine(OBJFileReadStream& context, const std::vector<std::string>& words) const
{
    if (words.size() < 2)
    {
        LOG_ERROR("The line usemtl is not full");
        throw FileError("File reading error");
    }

    auto model_ptr = context.getModelPtr();
    auto material_name_to_idx = context.getMaterialNameToIdx();
    auto current_mesh = context.getCurrentMesh();

    auto& final_meshes = model_ptr->meshes;
    auto& final_indices = model_ptr->indices;

    std::string material_name = words[1];

    int material_idx = -1;
    if (material_name_to_idx.contains(material_name))
    {
        material_idx = material_name_to_idx[material_name];
    }
    else
    {
        LOG_WARN("Material '{}' not found in .mtl file", material_name);
    }

    final_meshes.emplace_back();
    current_mesh = &final_meshes.back();

    current_mesh->name = material_name;
    current_mesh->materialIdx = material_idx;
    current_mesh->startIndexLoc = final_indices.size();

    current_mesh->indexCount = 0;
}
