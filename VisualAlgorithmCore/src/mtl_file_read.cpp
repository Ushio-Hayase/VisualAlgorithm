#include "mtl_file_read.h"

#include "exception.h"
#include "log_macro.h"
#include "util.h"

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

std::unordered_map<std::string, MtlKeyword> mtlKeywordMap = {
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

void MTLFileReadStream::loadFromFile(std::string fileName, Model* const modelPtr)
{
    if (!modelPtr)
    {
        LOG_ERROR("model pointer is null!");
        throw NullPointerError();
    }

    this->fileName = fileName;
    fs = std::ifstream{fileName};

    if (!fs.is_open())
    {
        LOG_ERROR("Can't open mtl file, file name is {}", this->fileName);
        throw FileError("can't open file");
    }

    std::string line;

    auto& finalMaterials = modelPtr->materials;

    while (std::getline(fs, line))
    {
        using namespace utils;
        std::vector<std::string> words = split_string(line, ' ');

        std::string& key_word = words[0];

        if (!mtlKeywordMap.contains(key_word))
        {
            continue;
        }

        auto& currentMaterial = finalMaterials.back();

        switch (mtlKeywordMap[key_word])
        {
        case MtlKeyword::newmtl: {
            finalMaterials.emplace_back();
            if (words.size() < 2)
            {
                LOG_ERROR("Can't find material name");
                throw FileError("Can't find material name");
            }
            finalMaterials.back().name = words[1];
            break;
        }
        case MtlKeyword::Ka: {
            wordsLengthCheck(words.size(), 4, "Ka");
            currentMaterial.ambientColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Kd: {
            wordsLengthCheck(words.size(), 4, "Kd");
            currentMaterial.diffuseColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ks: {
            wordsLengthCheck(words.size(), 4, "Ks");
            currentMaterial.specularColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ke: {
            wordsLengthCheck(words.size(), 4, "Ke");
            currentMaterial.emissiveColor = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Tf: {
            wordsLengthCheck(words.size(), 4, "Tf");
            currentMaterial.transmissionFilter = stov3(words[1], words[2], words[3]);
            break;
        }
        case MtlKeyword::Ns: {
            wordsLengthCheck(words.size(), 2, "Ns");
            currentMaterial.shininess = std::stof(words[1]);
            break;
        }
        case MtlKeyword::d: {
            wordsLengthCheck(words.size(), 2, "d");
            currentMaterial.opacity = std::stof(words[1]);
            break;
        }
        case MtlKeyword::Tr: {
            wordsLengthCheck(words.size(), 2, "Tr");
            currentMaterial.opacity = 1 - std::stof(words[1]);
            break;
        }
        case MtlKeyword::Ni: {
            wordsLengthCheck(words.size(), 2, "Ni");
            currentMaterial.indexOfRefraction = std::stof(words[1]);
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
