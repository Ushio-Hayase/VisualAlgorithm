//
// Created by UshioHayase on 2025-10-22.
//

#ifndef VISUALALGORITHMCORE_UTIL_H
#define VISUALALGORITHMCORE_UTIL_H

#include "Vec.h"

#include <string>
#include <vector>

namespace utils
{
/**
 * @param str 분할할 문자열
 * @param ch 자를 기준으로 삼을 문자
 * @return 기준 문자로 자른 문자열 배열
 */
std::vector<std::string> split_string(const std::string& str, const char ch);

/** @brief 문자열 2개를 Vector2로 바꿉니다
 */
inline Vector2 stov2(const std::string& x, const std::string y)
{
    return {std::stof(x), std::stof(y)};
}

/** @brief 문자열 3개를 Vector3로 바꿉니다
 */
inline Vector3 stov3(const std::string& x, const std::string& y, const std::string& z)
{
    return {std::stof(x), std::stof(y), std::stof(z)};
}
} // namespace utils

#endif // VISUALALGORITHMCORE_UTIL_H
