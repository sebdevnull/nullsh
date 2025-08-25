/**
 * @file shell.cpp
 * @brief NullShell class implementation
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/shell.h"

#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "nullsh/parser.h"
#include "nullsh/result_capturer.h"
#include "nullsh/util.h"

namespace nullsh::shell
{
    /**
     * @brief Runs the interactive shell
     *
     * @return int Exit code
     */
    int NullShell::run()
    {
        running = true;
        while (running)
        {
            std::cout << prompt << std::flush;

            std::string line;
            if (!std::getline(std::cin, line))
            {
                break;
            }

            auto tokens = util::tokenize(line);
            if (!tokens)
            {
                std::cerr << "parse error: " << tokens.error() << "\n";
                continue;
            }

            int rc = dispatch(*tokens);
            (void) rc; // reserved for later
        }

        return -1;
    }

    /**
     * @brief Dispatches a command line to the appropriate handler
     *
     * @param args Tokenized command line
     * @return int Exit code
     */
    int NullShell::dispatch(const std::vector<std::string>& args)
    {
        if (args.empty())
        {
            return 0;
        }

        auto cmd = parser::parse_command(args);
        auto res = execute(cmd);

        if (cmd.ops.empty())
        {
            // push default operator if none specified
            cmd.ops.push_back(command::Op::None);
        }

        for (const auto& op : cmd.ops)
        {
            command::apply_operator(op, res);
        }

        return res.return_code;
    }

    /**
     * @brief Executes a command
     *
     * @param cmd Command to execute
     * @return command::CommandResult Result of command execution
     */
    command::CommandResult NullShell::execute(command::Command& cmd)
    {
        command::CommandResult res {};
        io::CommandResultCapturer cmd_capturer {res};

        if (cmd.args.empty())
        {
            return res;
        }

        std::vector<char*> cstr_args;
        cstr_args.reserve(cmd.args.size() + 1);
        for (auto& arg : cmd.args)
        {
            cstr_args.push_back(arg.data());
        }
        cstr_args.push_back(nullptr);

        cmd_capturer.init_pipes();

        pid_t pid = fork();
        if (pid < 0)
        {
            std::perror("fork");
            res.return_code = EXIT_CMD_NOT_FOUND;
            return res;
        }
        if (pid == 0)
        {
            cmd_capturer.prepare_child();

            execvp(cstr_args[0], cstr_args.data());
            if (errno == ENOENT)
            {
                _exit(EXIT_CMD_NOT_FOUND);
            }
            else
            {
                _exit(EXIT_CMD_NOT_EXECUTABLE);
            }
        }

        cmd_capturer.capture_parent(pid);

        return res;
    }

} // namespace nullsh::shell