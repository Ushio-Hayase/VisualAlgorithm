//
// Created by UshioHayase on 2025-10-22.
//

#include "util.h"

namespace utils
{
std::vector<std::string> split_string(const std::string& str, const char ch)
{
    int cut_idx = 0;

    std::vector<std::string> result;

    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == ch)
        {
            result.emplace_back(str, cut_idx, i - cut_idx + 1);
            cut_idx = i;
        }
    }

    return result;
}

}  // namespace utils
