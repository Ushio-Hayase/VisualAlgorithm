//
// Created by UshioHayase on 2025-10-15.
//

#ifndef VISUALALGORITHMCORE_MODEL_H
#define VISUALALGORITHMCORE_MODEL_H

#include <string>
#include <vector>

#include "vector.h"

struct Vertex
{
    Vector3 position;
    Vector2 texture_coor;
    Vector3 normal;
};

struct Material
{
    Vector3 color;
    Vector3 diffuse;
    Vector3 specular;
    float emissive;
    float sharpness;
};

struct Mesh
{
    std::vector<unsigned int> indices;
    std::string texture_file_name;
    Material* material;
    unsigned int material_idx;
};

struct Model
{
    std::vector<Vertex> vertex;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
};

#endif  // VISUALALGORITHMCORE_MODEL_H
