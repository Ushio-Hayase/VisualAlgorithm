//
// Created by UshioHayase on 2025-10-15.
//

#ifndef VISUALALGORITHMCORE_MODEL_H
#define VISUALALGORITHMCORE_MODEL_H

#include "vec.h"

#include <string>
#include <vector>

struct Vertex
{
    Vector3 position;
    Vector2 textureCoor;
    Vector3 normal;
};

struct Material
{
    std::string name;
    Vector3 ambientColor;       // Ka - Ambient Color (주변광)
    Vector3 diffuseColor;       // Kd - Diffuse Color (확산광)
    Vector3 specularColor;      // Ks - Specular Color (반사광)
    Vector3 emissiveColor;      // Ke - Emissive Color (방출광)
    float shininess;            // Ns - Specular Exponent (반사광의 날카로움)
    float indexOfRefraction;    // Ni - index of Refraction (굴절률)
    float opacity;              // d - Opacity (불투명도)
    Vector3 transmissionFilter; // Tf - Transmission Filter (투과 색상)
    int illuminationModel;      // illum - Illumination Model (조명 모델 번호)
};

struct Mesh
{
    uint32_t indexCount;
    uint32_t startIndexLoc;
    std::string name;
    Material* material;
    uint32_t materialIdx;
};

struct Model
{
    std::vector<Vertex> vertex;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<uint32_t> indices;
};

#endif // VISUALALGORITHMCORE_MODEL_H
