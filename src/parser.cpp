/**
 * @file parser.cpp
 * @brief Parser for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/parser.h"

#include "nullsh/command.h"

namespace nullsh::parser
{
    auto parse_operator(const std::string_view token) -> nullsh::command::Op
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
        else if (token == "$??"sv)
        {
            op = command::Op::PrintRCHuman;
        }

        return op;
    }

    auto parse_command(const std::vector<std::string>& tokens) -> nullsh::command::Command
    {
        nullsh::command::Command cmd {};

        for (const auto& token : tokens)
        {
            if (auto op = parse_operator(token); op != command::Op::None)
            {
                cmd.ops.push_back(op);
            }
            else
            {
                cmd.args.push_back(token);
            }
        }

        return cmd;
    }
} // namespace nullsh::parser