/**
 * @file parser.h
 * @brief Parser for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include "nullsh/command.h"

namespace nullsh::parser
{
    auto parse_operator(std::string_view token) -> nullsh::command::Op;
    auto parse_command(const std::vector<std::string>& tokens) -> nullsh::command::Command;

} // namespace nullsh::parser