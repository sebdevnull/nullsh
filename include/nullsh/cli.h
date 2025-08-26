/**
 * @file cli.h
 * @brief CLI for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <expected>
#include <optional>
#include <span>
#include <string>

namespace nullsh::cli
{
    struct CLI
    {
        std::optional<std::string> one_shot;
        std::optional<std::string> spawn_term;
    };

    auto parse_cli(std::span<const char*> args) -> std::expected<CLI, std::string>;

} // namespace nullsh::cli