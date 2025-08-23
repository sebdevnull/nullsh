/**
 * @file cli.h
 * @brief CLI for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

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

    std::expected<CLI, std::string> parse_cli(int argc, char **argv);

} // namespace nullsh