/**
 * @file cli.h
 * @brief CLI for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <span>
#include <string>
#include <optional>
#include <expected>

namespace nullsh::cli
{
    struct CLI
    {
        std::optional<std::string> one_shot;
        std::optional<std::string> spawn_term;
    };

    auto parse_cli(std::span<const char *> args) -> std::expected<CLI, std::string>;

} // namespace nullsh