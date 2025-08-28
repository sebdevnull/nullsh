/**
 * @file shell.h
 * @brief NullShell class definition
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <vector>

#include "nullsh/command.h"

namespace nullsh::shell
{
    // Conventional POSIX exit codes for shells
    constexpr int EXIT_CMD_NOT_EXECUTABLE = 126; // found but not executable
    constexpr int EXIT_CMD_NOT_FOUND = 127;      // command not found
    constexpr int EXIT_SIGNAL_BASE = 128;        // 128 + signal number

    class NullShell
    {
        bool running {false};

      public:
        NullShell() = default;

        int run();
        int execute(const std::vector<std::string>& args);

      private:
        std::string prompt {"nullsh>"};
        int last_status_ {0};

        command::CommandResult execute_command(command::Command& cmd);
    };
} // namespace nullsh::shell