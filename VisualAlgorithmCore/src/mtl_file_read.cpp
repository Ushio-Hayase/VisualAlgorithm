#include "mtl_file_read.h"

#include <unordered_map>

#include "exception.h"
#include "util.h"

namespace
{
enum class MtlKeyword
{
    newmtl,  // 새로운 재질 정의
    Ka,      // 주변광(Ambient), RGB값
    Kd,      // 분산광(Diffuse Color), RGB값
    Ks,      // 반사광(Specular Color), RGB값
    Ke,      // 방출광(Emissive Color), RGB값
    Tf,      // 투과 필터(Transmission Filter), RGB값
    Ns,      // 반사지수(Specular Exponent), 0-1000
    d,       // 불투명도(Dissolve), 1.0이 완전 불투명
    Tr,      // 투명도(Transparency), 1.0이 완전 투명
    Ni       // 굴절률(Optical Density), 1.0은 굴절이 없음
};

std::unordered_map<std::string, MtlKeyword> mtlKeywordMap = {
    {"newmtl", MtlKeyword::newmtl}, {"Ka", MtlKeyword::Ka}, {"Kd", MtlKeyword::Kd}, {"Ks", MtlKeyword::Ks},
    {"Ke", MtlKeyword::Ke},         {"Tf", MtlKeyword::Tf}, {"Ns", MtlKeyword::Ns}, {"d", MtlKeyword::d},
    {"Tr", MtlKeyword::Tr},         {"Ni", MtlKeyword::Ni}};
}  // namespace

void MTLFileReadStream::read(Model* const modelPtr)
{
    if (!modelPtr)
    {
        throw NullPointerError();
    }

    if (!fs_.is_open())
    {
        throw FileError("can't open file");
    }

    std::string line;

    auto& finalMaterials = modelPtr->materials;

    while (std::getline(fs_, line))
    {
        using namespace utils;
        std::vector<std::string> words = split_string(line, ' ');

        std::string& key_word = words[0];

        if (!mtlKeywordMap.contains(key_word))
        {
            continue;
        }

        auto& currentMaterial = finalMaterials.back();

        switch (MtlKeyword& head = mtlKeywordMap[key_word])
        {
            case MtlKeyword::newmtl:
            {
                finalMaterials.emplace_back();
                break;
            }
            case MtlKeyword::Ka:
            {
                currentMaterial.ambientColor = stov3(words[1], words[2], words[3]);
                break;
            }
            case MtlKeyword::Kd:
            {
                currentMaterial.diffuseColor = stov3(words[1], words[2], words[3]);
                break;
            }
            case MtlKeyword::Ks:
            {
                currentMaterial.specularColor = stov3(words[1], words[2], words[3]);
                break;
            }
            case MtlKeyword::Ke:
            {
                currentMaterial.emissiveColor = stov3(words[1], words[2], words[3]);
                break;
            }
            case MtlKeyword::Tf:
            {
                currentMaterial.transmissionFilter = stov3(words[1], words[2], words[3]);
                break;
            }
            case MtlKeyword::Ns:
            {
                currentMaterial.shininess = std::stof(words[1]);
                break;
            }
            case MtlKeyword::d:
            {
                currentMaterial.opacity = std::stof(words[1]);
                break;
            }
            case MtlKeyword::Tr:
            {
                currentMaterial.opacity = 1 - std::stof(words[1]);
            }
            case MtlKeyword::Ni:
            {
                currentMaterial.indexOfRefraction = std::stof(words[1]);
            }
            default:
            {
                break;
            }
        }
    }
}
