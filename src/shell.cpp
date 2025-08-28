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
#include <unordered_map>

#include "nullsh/builtins.h"
#include "nullsh/command.h"
#include "nullsh/executor.h"
#include "nullsh/parser.h"
#include "nullsh/util.h"

namespace nullsh::shell
{
    using ExecutorFn = command::CommandResult (*)(command::Command&, NullShell&);

    static const std::unordered_map<command::CommandType, ExecutorFn> DISPATCH_TABLE = {
        {command::CommandType::Builtin,
         [](auto& cmd, auto& sh) { return builtins::execute(cmd, sh); }},
        {command::CommandType::External,
         [](auto& cmd, auto&) { return executor::exec_external(cmd); }},
    };

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

            int rc = execute(*tokens);
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
    int NullShell::execute(const std::vector<std::string>& args)
    {
        auto cmd = parser::make_command(args);
        if (!cmd)
        {
            return 0;
        }

        auto res = execute_command(*cmd);
        last_status_ = res.return_code;

        return res.return_code;
    }

    /**
     * @brief Executes a command
     *
     * @param cmd Command to execute
     * @return command::CommandResult Result of command execution
     */
    command::CommandResult NullShell::execute_command(command::Command& cmd)
    {
        if (auto it = DISPATCH_TABLE.find(cmd.type); it != DISPATCH_TABLE.end())
        {
            auto res = it->second(cmd, *this);

            for (auto op : cmd.ops)
            {
                executor::apply_operator(op, res);
            }

            return res;
        }

        return {
            .return_code = EXIT_CMD_NOT_FOUND, .stdout_data = "", .stderr_data = "Unknown command"};
    }

} // namespace nullsh::shell