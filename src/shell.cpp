/**
 * @file shell.cpp
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "nullsh/shell.h"
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
        while (1)
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
            (void)rc; // reserved for later
        }

        return -1;
    }

    /**
     * @brief Dispatches a command line to the appropriate handler
     *
     * @param args Tokenized command line
     * @return int Exit code
     */
    int NullShell::dispatch(const std::vector<std::string> &args)
    {
        if (args.empty())
            return 0;

        return execute(args);
    }

    /**
     * @brief Executes a command
     *
     * @param args Tokenized command line
     * @return int Exit code
     */
    int NullShell::execute(const std::vector<std::string> &args)
    {
        if (args.empty())
            return 0;

        std::vector<char *> cstr_args;
        cstr_args.reserve(args.size() + 1);
        for (const auto &arg : args)
        {
            cstr_args.push_back(const_cast<char *>(arg.c_str()));
        }
        cstr_args.push_back(nullptr);

        pid_t pid = fork();
        if (pid < 0)
        {
            std::perror("fork");
            return EXIT_CMD_NOT_FOUND;
        }
        else if (pid == 0)
        {
            // Child process
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
        int status;
        if (waitpid(pid, &status, 0) < 0)
        {
            std::perror("waitpid");
            return EXIT_CMD_NOT_FOUND;
        }

        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            std::cerr << "Process terminated by signal " << WTERMSIG(status) << "\n";
            return EXIT_SIGNAL_BASE + WTERMSIG(status);
        }

        return 0;
    }

}