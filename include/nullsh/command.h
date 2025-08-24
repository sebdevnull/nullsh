/**
 * @file command.h
 * @brief Command structure and operations
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nullsh::command
{
    enum class Op : std::uint8_t
    {
        None,          // default
        ForceOutput,   // ! -> force print stdout
        DiscardOutput, // ? -> discard stdout + stderr
        PrintRC,       // $? -> print return code
        PrintRCHuman,  // $?? -> human-readable RC
    };

    struct Command
    {
        std::vector<std::string> args;
        std::vector<Op> ops;
    };

    struct CommandResult
    {
        int return_code;
        std::string stdout_data;
        std::string stderr_data;
    };

    void apply_operator(Op op, CommandResult& res);
} // namespace nullsh::command