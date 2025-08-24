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

namespace nullsh
{
    class NullShell
    {
        bool running {false};

      public:
        NullShell() = default;

        int run();
        int dispatch(const std::vector<std::string>& args);

      private:
        std::string prompt {"nullsh>"};

        command::CommandResult execute(command::Command& cmd);
    };
} // namespace nullsh