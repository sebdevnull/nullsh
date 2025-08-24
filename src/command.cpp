/**
 * @file command.cpp
 * @brief Command structure and operations
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/command.h"

#include <iostream>

namespace nullsh::command
{
    void apply_operator(Op op, CommandResult& res)
    {
        switch (op)
        {
            case Op::ForceOutput:
                // print stdout/stderr explicitly
                std::cout << res.stdout_data;
                std::cerr << res.stderr_data;
                break;
            case Op::DiscardOutput:
                // drop output
                res.stdout_data.clear();
                res.stderr_data.clear();
                break;
            case Op::PrintRC:
                std::cout << res.return_code;
                break;
            case Op::PrintRCHuman:
                std::cout << res.return_code;
                if (res.return_code == 0)
                {
                    std::cout << " (success)";
                }
                else
                {
                    std::cout << " (failure)";
                }
                std::cout << "\n";
                break;
            case Op::None:
            default:
                // default: silent execution (stdout > /dev/null, stderr visible)
                res.stdout_data.clear();
                std::cerr << res.stderr_data;
                break;
        }
    }

} // namespace nullsh::command