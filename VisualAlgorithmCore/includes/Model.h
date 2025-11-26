//
// Created by UshioHayase on 2025-10-15.
//

#ifndef VISUALALGORITHMCORE_MODEL_H
#define VISUALALGORITHMCORE_MODEL_H

#include "Vec.h"

#include <string>
#include <vector>

struct Vertex
{
    Vector3 position;
    Vector2 texture_coor;
    Vector3 normal;
};

struct Material
{
    std::string name;
    Vector3 ambient_color;       // Ka - Ambient Color (주변광)
    Vector3 diffuse_color;       // Kd - Diffuse Color (확산광)
    Vector3 specular_color;      // Ks - Specular Color (반사광)
    Vector3 emissive_color;      // Ke - Emissive Color (방출광)
    float shininess;            // Ns - Specular Exponent (반사광의 날카로움)
    float index_of_refraction;    // Ni - index of Refraction (굴절률)
    float opacity;              // d - Opacity (불투명도)
    Vector3 transmission_filter; // Tf - Transmission Filter (투과 색상)
    int illumination_model;      // illum - Illumination Model (조명 모델 번호)
};

struct Mesh
{
    uint32_t index_count;
    uint32_t start_index_loc;
    std::string name;
    Material* material;
    uint32_t material_idx;
};

struct Model
{
    std::vector<Vertex> vertices;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<uint32_t> indices;
};

#endif // VISUALALGORITHMCORE_MODEL_H
