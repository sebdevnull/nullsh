/**
 * @file command.cpp
 * @brief Command structure and operations
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/command.h"

#include "nullsh/util.h"

namespace nullsh::command
{
    void sanitize_result(CommandResult& res)
    {
        util::newline(res.stdout_data);
        util::newline(res.stderr_data);
    }
} // namespace nullsh::command