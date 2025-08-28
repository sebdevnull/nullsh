/**
 * @file builtins.h
 * @brief Built-in commands
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>

#include "nullsh/command.h"
#include "nullsh/shell.h"

namespace nullsh::builtins
{
    bool is_builtin(const std::string& name);

    command::CommandResult execute(command::Command& cmd, shell::NullShell& sh);
} // namespace nullsh::builtins