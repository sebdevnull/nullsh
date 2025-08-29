/**
 * @file parser.cpp
 * @brief Parser for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/parser.h"

#include <optional>

#include "nullsh/builtins.h"
#include "nullsh/command.h"

namespace nullsh::parser
{
    auto parse_operator(std::string_view token) -> command::Op
    {
        using namespace std::literals;

        command::Op op = command::Op::None;

        if (token == "!"sv)
        {
            op = command::Op::ForceOutput;
        }
        else if (token == "?"sv)
        {
            op = command::Op::DiscardOutput;
        }
        else if (token == "$?"sv)
        {
            op = command::Op::PrintRC;
        }
        else if (token == "$$?"sv)
        {
            op = command::Op::PrintRCHuman;
        }

        return op;
    }

    std::optional<command::Command> make_command(const std::vector<std::string>& args)
    {
        if (args.empty())
        {
            return std::nullopt;
        }

        command::Command cmd {};
        cmd.name = args[0];
        cmd.args.assign(args.begin() + 1, args.end());

        if (builtins::is_builtin(cmd.name))
        {
            cmd.type = command::CommandType::Builtin;
        }
        else
        {
            cmd.type = command::CommandType::External;
        }

        if (!cmd.args.empty())
        {
            command::Op op = command::Op::None;
            while (!cmd.args.empty() && (op = parse_operator(cmd.args.back())) != command::Op::None)
            {
                cmd.ops.insert(cmd.ops.begin(), op);
                cmd.args.pop_back();
            }

            if (cmd.ops.empty())
            {
                cmd.ops.push_back(command::Op::None);
            }
        }

        return cmd;
    }

} // namespace nullsh::parser