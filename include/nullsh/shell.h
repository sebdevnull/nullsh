/**
 * @file shell.h
 * @brief 
 * 
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <vector>

namespace nullsh
{
    class NullShell
    {
    public:
        NullShell() {}

        int run();
        int dispatch(const std::vector<std::string> &args);

    private:
        std::string prompt{"nullsh>"};

        int execute(const std::vector<std::string> &args);
    };
} // namespace nullsh