/**
 * @file builtins.cpp
 * @brief Built-in commands
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/builtins.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "nullsh/shell.h"
#include "nullsh/util.h"

namespace nullsh::builtins
{
    using Handler = command::CommandResult (*)(command::Command&, shell::NullShell&);

    namespace
    {
        namespace fs = std::filesystem;

        command::CommandResult builtin_cd(command::Command& cmd,
                                          [[maybe_unused]] shell::NullShell& sh)
        {
            try
            {
                fs::path new_path;
                if (cmd.args.empty())
                {
                    auto home = util::get_env_var("HOME");

                    if (!home.has_value())
                    {
                        return {
                            .return_code = 1, .stdout_data = "", .stderr_data = "cd: HOME not set"};
                    }
                    new_path = fs::path(*home);
                }
                else if (cmd.args.size() > 1)
                {
                    return {.return_code = 1,
                            .stdout_data = "",
                            .stderr_data = "cd: too many arguments"};
                }
                else
                {
                    new_path = util::expand_user_path(cmd.args[0]);
                }

                fs::path resolved_path;
                auto ec = util::resolve_directory(new_path, resolved_path);
                if (ec)
                {
                    return {.return_code = ec.value(),
                            .stdout_data = "",
                            .stderr_data = "cd: " + new_path.string() + ": " + ec.message()};
                }

                // change directory
                fs::current_path(resolved_path);

                // set PWD environment variable
                setenv("PWD", resolved_path.c_str(), 1);

                return {.return_code = 0, .stdout_data = "", .stderr_data = ""};
            }
            catch (const fs::filesystem_error& e)
            {
                return {.return_code = 1,
                        .stdout_data = "",
                        .stderr_data = "cd: " + std::string(e.what())};
            }
        }

        command::CommandResult builtin_pwd([[maybe_unused]] command::Command& cmd,
                                           [[maybe_unused]] shell::NullShell& sh)
        {
            // namespace fs = std::filesystem;
            try
            {
                fs::path cwd = fs::current_path();
                return {.return_code = 0, .stdout_data = cwd.string() + "\n", .stderr_data = ""};
            }
            catch (const fs::filesystem_error& e)
            {
                return {.return_code = errno, .stdout_data = "", .stderr_data = e.what()};
            }
        }

        command::CommandResult builtin_exit(command::Command& cmd, shell::NullShell& sh)
        {
            int status = 0;
            if (!cmd.args.empty())
            {
                try
                {
                    status = std::stoi(cmd.args[0]);
                }
                catch (const std::invalid_argument& e)
                {
                    return {.return_code = 1,
                            .stdout_data = "",
                            .stderr_data = "exit: numeric argument required"};
                }
                catch (const std::out_of_range& e)
                {
                    return {.return_code = 1,
                            .stdout_data = "",
                            .stderr_data = "exit: numeric argument out of range"};
                }
            }
            sh.exit();
            return {.return_code = status, .stdout_data = "", .stderr_data = ""};
        }

        command::CommandResult builtin_echo(command::Command& cmd,
                                            [[maybe_unused]] shell::NullShell& sh)
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

    /**
     * @brief Checks if a command is a built-in
     *
     * @param name Command name
     * @return true if built-in, false otherwise
     */
    bool is_builtin(const std::string& name)
    {
        return BUILTINS_TABLE.contains(name);
    }

    /**
     * @brief Executes a built-in command
     *
     * @param cmd Command to execute
     * @param sh Shell context
     * @return command::CommandResult Result of command execution
     */
    command::CommandResult execute(command::Command& cmd, shell::NullShell& sh)
    {
        auto entry = BUILTINS_TABLE.find(cmd.name);
        if (entry != BUILTINS_TABLE.end())
        {
            return entry->second(cmd, sh);
        }

        return {.return_code = shell::EXIT_CMD_NOT_FOUND,
                .stdout_data = "",
                .stderr_data = "not a builtin"};
    }

} // namespace nullsh::builtins