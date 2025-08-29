/**
 * @file result_capturer.h
 * @brief Implementation of the interface IOCapturer for capturing command execution results.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <string>

#include "nullsh/capturer.h"
#include "nullsh/command.h"

namespace nullsh::io
{
    class CommandResultCapturer final : public IOCapturer
    {
        static constexpr int BUF_SIZE = 4096;

      public:
        explicit CommandResultCapturer(command::CommandResult& res) : cmd_result(&res) {}

        void init_pipes();
        void prepare_child() override;
        void capture_parent(pid_t pid) override;

      private:
        command::CommandResult* cmd_result;
        std::array<int, 2> stdout_pipe {-1, -1};
        std::array<int, 2> stderr_pipe {-1, -1};
        std::array<char, BUF_SIZE> buffer {};

        void read_pipe(int fd, std::string& out);
    };
} // namespace nullsh::io