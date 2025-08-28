/**
 * @file command.h
 * @brief Command structure and operations
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <vector>

namespace nullsh::command
{
    enum class Op
    {
        None,          // default
        ForceOutput,   // ! -> force print stdout
        DiscardOutput, // ? -> discard stdout + stderr
        PrintRC,       // $? -> print return code
        PrintRCHuman,  // $$? -> human-readable RC
    };

    enum class CommandType
    {
        Builtin,
        External
    };

    struct Command
    {
        CommandType type;
        std::string name;
        std::vector<std::string> args;
        std::vector<Op> ops;
    };

    struct CommandResult
    {
        int return_code;
        std::string stdout_data;
        std::string stderr_data;
    };
} // namespace nullsh::command