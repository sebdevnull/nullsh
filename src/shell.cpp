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
#include "nullsh/util.h"

// Conventional POSIX exit codes for shells
constexpr int EXIT_CMD_NOT_EXECUTABLE = 126; // found but not executable
constexpr int EXIT_CMD_NOT_FOUND = 127;      // command not found
constexpr int EXIT_SIGNAL_BASE = 128;        // 128 + signal number

namespace nullsh
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

        if (cmd.args.empty())
        {
            return res;
        }

        std::vector<char*> cstr_args;
        cstr_args.reserve(cmd.args.size() + 1);
        for (const auto& arg : cmd.args)
        {
            cstr_args.push_back(const_cast<char*>(arg.c_str()));
        }
        cstr_args.push_back(nullptr);

        int stdout_pipe[2], stderr_pipe[2];
        if (pipe(stdout_pipe) < 0 || pipe(stderr_pipe) < 0)
        {
            std::perror("pipe");
            res.return_code = EXIT_CMD_NOT_FOUND;
            return res;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            std::perror("fork");
            res.return_code = EXIT_CMD_NOT_FOUND;
            return res;
        }
        if (pid == 0)
        {
            // Child process
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);
            dup2(stdout_pipe[1], STDOUT_FILENO);
            dup2(stderr_pipe[1], STDERR_FILENO);
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);

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

        // Parent process
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // capture child output into command result
        res.return_code = 0;
        {
            char buffer[4096];
            ssize_t count;
            while ((count = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0)
            {
                res.stdout_data.append(buffer, count);
            }
        }
        {
            char buffer[4096];
            ssize_t count;
            while ((count = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0)
            {
                res.stderr_data.append(buffer, count);
            }
        }
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        int status = 0;
        if (waitpid(pid, &status, 0) < 0)
        {
            std::perror("waitpid");
            res.return_code = EXIT_CMD_NOT_FOUND;
            return res;
        }

        if (WIFEXITED(status))
        {
            res.return_code = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            std::cerr << "Process terminated by signal " << WTERMSIG(status) << "\n";
            res.return_code = EXIT_SIGNAL_BASE + WTERMSIG(status);
        }

        return res;
    }

} // namespace nullsh