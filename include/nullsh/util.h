/**
 * @file util.h
 * @brief Common utility components
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace nullsh::util
{
    // String helpers
    void ltrim(std::string& str);
    void rtrim(std::string& str);
    void trim(std::string& str);
    void newline(std::string& str);
    auto tokenize(std::string_view line) -> std::expected<std::vector<std::string>, std::string>;

    // Command helpers
    bool command_exists(const std::string& cmd);

    // Filesystem helpers
    auto get_env_var(const std::string& name) -> std::optional<std::string>;
    auto expand_user_path(const std::string& path) -> std::filesystem::path;
    std::error_code resolve_directory(const std::filesystem::path& path,
                                      std::filesystem::path& resolved_path);
} // namespace nullsh::util