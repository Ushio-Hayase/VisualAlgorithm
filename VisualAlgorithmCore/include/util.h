//
// Created by UshioHayase on 2025-10-22.
//

#ifndef VISUALALGORITHMCORE_UTIL_H
#define VISUALALGORITHMCORE_UTIL_H

#include <string>
#include <vector>

namespace utils
{
/**
 *
 * @param str 분할할 문자열
 * @param ch 자를 기준으로 삼을 문자
 * @return 기준 문자로 자른 문자열 배열
 */
std::vector<std::string> split_string(const std::string& str, const char ch);
}  // namespace utils

#endif  // VISUALALGORITHMCORE_UTIL_H
