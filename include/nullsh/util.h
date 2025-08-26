/**
 * @file util.h
 * @brief Common utility components
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace nullsh::util
{
    void ltrim(std::string& str);
    void rtrim(std::string& str);
    void trim(std::string& str);
    std::expected<std::vector<std::string>, std::string> tokenize(std::string_view line);
    bool command_exists(const std::string& cmd);
} // namespace nullsh::util