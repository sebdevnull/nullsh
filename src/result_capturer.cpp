/**
 * @file result_capturer.cpp
 * @brief Implementation of the interface IOCapturer for capturing command execution results.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/result_capturer.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>
#include <stdexcept>

#include "nullsh/shell.h"

namespace nullsh::io
{
    void CommandResultCapturer::init_pipes()
    {
        if (pipe(stdout_pipe.data()) < 0 || pipe(stderr_pipe.data()) < 0)
        {
            throw std::runtime_error("Failed to create pipes");
        }
    }

    void CommandResultCapturer::prepare_child()
    {
        // redirect child stdio to pipes
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
    }

    void CommandResultCapturer::capture_parent(pid_t pid)
    {
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        read_pipe(stdout_pipe[0], res->stdout_data);
        read_pipe(stderr_pipe[0], res->stderr_data);

        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        int status = 0;
        if (waitpid(pid, &status, 0) < 0)
        {
            std::perror("waitpid");
            res->return_code = nullsh::shell::EXIT_CMD_NOT_FOUND;
            return;
        }

        if (WIFEXITED(status))
        {
            res->return_code = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            std::cerr << "Process terminated by signal " << WTERMSIG(status) << "\n";
            res->return_code = nullsh::shell::EXIT_SIGNAL_BASE + WTERMSIG(status);
        }
    }

    // ===== Private functions =====
    void CommandResultCapturer::read_pipe(int fd, std::string& out)
    {
        ssize_t count = 0;
        while ((count = read(fd, buffer.data(), sizeof(buffer))) > 0)
        {
            out.append(buffer.data(), count);
        }
    }
} // namespace nullsh::io