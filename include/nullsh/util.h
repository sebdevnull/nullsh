/**
 * @file util.h
 * @brief Common utility components
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <expected>
#include <vector>
#include <string_view>

namespace nullsh::util
{
    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);

    std::expected<std::vector<std::string>, std::string> tokenize(std::string_view line);
}