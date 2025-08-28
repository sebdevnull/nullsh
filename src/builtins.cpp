/**
 * @file builtins.cpp
 * @brief Built-in commands
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/builtins.h"

#include <linux/limits.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <ranges>
#include <string>
#include <unordered_map>

#include "nullsh/shell.h"

namespace nullsh::builtins
{
    using Handler = command::CommandResult (*)(command::Command&);

    namespace
    {
        command::CommandResult builtin_cd(command::Command& cmd)
        {
            std::string path;

            if (cmd.args.empty())
            {
                path = getenv("HOME");

                if (path.empty())
                {
                    return {.return_code = 1, .stdout_data = "", .stderr_data = "cd: HOME not set"};
                }
            }
            else
            {
                path = cmd.args[0];
            }

            if (chdir(path.c_str()) != 0)
            {
                return {.return_code = errno,
                        .stdout_data = "",
                        .stderr_data = std::string("cd: ") + strerror(errno)};
            }

            // char cwd[PATH_MAX];
            // if (getcwd(cwd, sizeof(cwd)))
            // {
            //     setenv("PWD", cwd, 1);
            // }

            return {.return_code = 0, .stdout_data = "", .stderr_data = ""};
        }

        command::CommandResult builtin_pwd([[maybe_unused]] command::Command& cmd)
        {
            std::string cwd(PATH_MAX, '\0');
            if (getcwd(cwd.data(), cwd.size()) != nullptr)
            {
                return {.return_code = 0, .stdout_data = cwd + "\n", .stderr_data = ""};
            }

            return {
                .return_code = errno, .stdout_data = "", .stderr_data = "pwd: failed to getcwd"};
        }

        command::CommandResult builtin_exit(command::Command& cmd)
        {
            int status = 0;
            if (!cmd.args.empty())
            {
                status = std::stoi(cmd.args[0]);
            }
            std::exit(status);
        }

        command::CommandResult builtin_echo(command::Command& cmd)
        {
            auto joined = cmd.args | std::views::join_with(' ');
            auto out = std::format("{}\n", std::string(joined.begin(), joined.end()));
            return {.return_code = 0, .stdout_data = out, .stderr_data = ""};
        }
    } // namespace

    // builtin dispatch table
    static const std::unordered_map<std::string, Handler> BUILTINS_TABLE = {
        {"cd", &builtin_cd},     //
        {"pwd", &builtin_pwd},   //
        {"exit", &builtin_exit}, //
        {"echo", &builtin_echo}  //
    };

    bool is_builtin(const std::string& name)
    {
        return BUILTINS_TABLE.contains(name);
    }

    command::CommandResult execute(command::Command& cmd, [[maybe_unused]] shell::NullShell& sh)
    {
        auto entry = BUILTINS_TABLE.find(cmd.name);
        if (entry != BUILTINS_TABLE.end())
        {
            return entry->second(cmd);
        }

        return {.return_code = shell::EXIT_CMD_NOT_FOUND,
                .stdout_data = "",
                .stderr_data = "not a builtin"};
    }

} // namespace nullsh::builtins