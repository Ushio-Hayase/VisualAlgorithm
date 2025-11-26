#include "MtlFileRead.h"

#include "Exception.h"
#include "LogMacro.h"
#include "Util.h"

#include <unordered_map>

namespace
{
enum class MtlKeyword
{
    newmtl, // 새로운 재질 정의
    Ka,     // 주변광(Ambient), RGB값
    Kd,     // 분산광(Diffuse Color), RGB값
    Ks,     // 반사광(Specular Color), RGB값
    Ke,     // 방출광(Emissive Color), RGB값
    Tf,     // 투과 필터(Transmission Filter), RGB값
    Ns,     // 반사지수(Specular Exponent), 0-1000
    d,      // 불투명도(Dissolve), 1.0이 완전 불투명
    Tr,     // 투명도(Transparency), 1.0이 완전 투명
    Ni      // 굴절률(Optical Density), 1.0은 굴절이 없음
};

std::unordered_map<std::string, MtlKeyword> mtl_keyword_map = {
    {"newmtl", MtlKeyword::newmtl}, {"Ka", MtlKeyword::Ka}, {"Kd", MtlKeyword::Kd}, {"Ks", MtlKeyword::Ks},
    {"Ke", MtlKeyword::Ke},         {"Tf", MtlKeyword::Tf}, {"Ns", MtlKeyword::Ns}, {"d", MtlKeyword::d},
    {"Tr", MtlKeyword::Tr},         {"Ni", MtlKeyword::Ni}};

__forceinline void wordsLengthCheck(const int src, const int criteria, const char* name)
{
    if (src < criteria)
    {
        LOG_WARN("The  {}isf{} line ar not filled up", name);
        throw FileError("File reading error");
    }
}
} // namespace

void MTLFileReadStream::loadFromFile(std::string file_name, Model* const model_ptr)
{
    if (!model_ptr)
    {
        LOG_ERROR("model pointer is null!");
        throw NullPointerError();
    }

    this->fileName = file_name;
    fs = std::ifstream{file_name};

    if (!fs.is_open())
    {
        LOG_ERROR("Can't open mtl file, file name is {}", this->fileName);
        throw FileError("can't open file");
    }

    std::string line;

    auto& final_materials = model_ptr->materials;

    while (std::getline(fs, line))
    {
        using namespace utils;
        std::vector<std::string> words = split_string(line, ' ');

        std::string& key_word = words[0];

        if (!mtl_keyword_map.contains(key_word))
        {
            continue;
        }

        auto& current_material = final_materials.back();

        switch (mtl_keyword_map[key_word])
        {
        case MtlKeyword::newmtl: {
            final_materials.emplace_back();
            if (words.size() < 2)
            {
                LOG_ERROR("Can't find material name");
                throw FileError("Can't find material name");
            }
            final_materials.back().name = words[1];
            break;
        }
        case MtlKeyword::Ka: {
            wordsLengthCheck(words.size(), 4, "Ka");
            current_material.ambientColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Kd: {
            wordsLengthCheck(words.size(), 4, "Kd");
            current_material.diffuseColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ks: {
            wordsLengthCheck(words.size(), 4, "Ks");
            current_material.specularColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ke: {
            wordsLengthCheck(words.size(), 4, "Ke");
            current_material.emissiveColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Tf: {
            wordsLengthCheck(words.size(), 4, "Tf");
            current_material.transmissionFilter = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ns: {
            wordsLengthCheck(words.size(), 2, "Ns");
            current_material.shininess = std::stof(words[1]);
            break;
        }
        case MtlKeyword::d: {
            wordsLengthCheck(words.size(), 2, "d");
            current_material.opacity = std::stof(words[1]);
            break;
        }
        case MtlKeyword::Tr: {
            wordsLengthCheck(words.size(), 2, "Tr");
            current_material.opacity = 1 - std::stof(words[1]);
            break;
        }
        case MtlKeyword::Ni: {
            wordsLengthCheck(words.size(), 2, "Ni");
            current_material.indexOfRefraction = std::stof(words[1]);
            break;
        }
        default: {
            break;
        }
        }
    }

    fs.close();
}

void MTLFileReadStream::initialize()
{
    fs.clear();
    fileName.clear();
}
