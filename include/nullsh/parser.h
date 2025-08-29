/**
 * @file parser.h
 * @brief Parser for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <optional>

#include "nullsh/command.h"

namespace nullsh::parser
{
    auto parse_operator(std::string_view token) -> command::Op;
    std::optional<command::Command> make_command(const std::vector<std::string>& args);

} // namespace nullsh::parser