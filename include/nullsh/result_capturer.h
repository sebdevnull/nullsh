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
        static constexpr int buf_size = 4096;

      public:
        explicit CommandResultCapturer(command::CommandResult& r) : res(&r) {}

        void init_pipes();
        void prepare_child() override;
        void capture_parent(pid_t pid) override;

      private:
        command::CommandResult* res;
        std::array<int, 2> stdout_pipe {-1, -1};
        std::array<int, 2> stderr_pipe {-1, -1};
        std::array<char, buf_size> buffer {};

        void read_pipe(int fd, std::string& out);
    };
} // namespace nullsh::io