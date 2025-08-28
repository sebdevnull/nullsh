/**
 * @file executor.h
 * @brief Command executor
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include "nullsh/command.h"

namespace nullsh::executor
{
    command::CommandResult exec_external(const command::Command& cmd);
    void apply_operator(command::Op op, command::CommandResult& res);

} // namespace nullsh::executor