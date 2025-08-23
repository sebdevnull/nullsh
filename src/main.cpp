/**
 * @file main.cpp
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <format>
#include <string>

#include "nullsh/cli.h"
#include "nullsh/shell.h"
#include "nullsh/util.h"

int main(int argc, char **argv)
{
    auto cli = nullsh::cli::parse_cli(argc, argv);

    if (!cli)
    {
        std::cerr << "nullsh: " << cli.error() << "\n";
        return 2;
    }

    nullsh::NullShell sh{};

    if (cli->one_shot)
    {
        auto tokens = nullsh::util::tokenize(*cli->one_shot);
        if (!tokens)
        {
            std::cerr << "parse error: " << tokens.error() << "\n";
            return 2;
        }

        return sh.dispatch(tokens.value());
    }

    if (!getenv("NULLSH_IN_TERMINAL") && cli->spawn_term)
    {
        // set to avoid infinite recursion
        setenv("NULLSH_IN_TERMINAL", "1", 1);

        std::string cmd = *cli->spawn_term;
        cmd += argv[0]; // path to nullsh

        for (int i = 1; i < argc; i++)
        {
            cmd += " ";
            cmd += argv[i];
        }

        system(cmd.c_str());
        return 0; // exit parent process
    }

    sh.run();

    return 0;
}