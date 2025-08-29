/**
 * @file executor.cpp
 * @brief Command executor
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/executor.h"

#include <unistd.h>

#include <iostream>
#include <vector>

#include "nullsh/result_capturer.h"
#include "nullsh/shell.h"

namespace nullsh::executor
{
    command::CommandResult exec_external(const command::Command& cmd)
    {
        command::CommandResult res {};
        io::CommandResultCapturer cmd_capturer {res};

        if (cmd.name.empty())
        {
            return res;
        }

        cmd_capturer.init_pipes();

        pid_t pid = fork();
        if (pid < 0)
        {
            std::perror("fork");
            res.return_code = shell::EXIT_CMD_NOT_FOUND;
            return res;
        }
        if (pid == 0)
        {
            std::vector<char*> argv;
            argv.reserve(cmd.args.size() + 2);
            // NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
            argv.push_back(const_cast<char*>(cmd.name.c_str()));
            for (const auto& arg : cmd.args)
            {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            // NOLINTEND(cppcoreguidelines-pro-type-const-cast)
            argv.push_back(nullptr);

            cmd_capturer.prepare_child();

            execvp(cmd.name.c_str(), argv.data());

            // if execvp returns it failed
            if (errno == ENOENT)
            {
                _exit(shell::EXIT_CMD_NOT_FOUND);
            }
            else
            {
                _exit(shell::EXIT_CMD_NOT_EXECUTABLE);
            }
        }

        cmd_capturer.capture_parent(pid);

        return res;
    }

    void apply_operator(command::Op op, command::CommandResult& res)
    {
        switch (op)
        {
            case command::Op::ForceOutput:
                // print stdout/stderr explicitly
                std::cout << res.stdout_data;
                std::cerr << res.stderr_data;
                break;
            case command::Op::DiscardOutput:
                // drop output
                res.stdout_data.clear();
                res.stderr_data.clear();
                break;
            case command::Op::PrintRC:
                std::cout << res.return_code << '\n';
                break;
            case command::Op::PrintRCHuman:
                std::cout << res.return_code;
                if (res.return_code == 0)
                {
                    std::cout << " (success)";
                }
                else
                {
                    std::cout << " (failure)";
                }
                std::cout << "\n";
                break;
            case command::Op::None:
            default:
                // default: silent execution (stdout > /dev/null, stderr visible)
                res.stdout_data.clear();
                std::cerr << res.stderr_data;
                break;
        }
    }
} // namespace nullsh::executor