/**
 * @file main.cpp
 * @brief Entry point for nullsh
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <span>
#include <string>

#include "nullsh/cli.h"
#include "nullsh/shell.h"
#include "nullsh/util.h"

int main(int argc, const char* argv[])
{
    std::span<const char*> args(argv, static_cast<std::size_t>(argc));

    auto cli = nullsh::cli::parse_cli(args);

    if (!cli)
    {
        std::cerr << "nullsh: " << cli.error() << "\n";
        return 2;
    }

    nullsh::shell::NullShell shell {};

    if (cli->one_shot)
    {
        auto tokens =
            nullsh::util::tokenize(*cli->one_shot); // NOLINT(bugprone-unchecked-optional-access)
        if (!tokens)
        {
            std::cerr << "parse error: " << tokens.error() << "\n";
            return 2;
        }

        return shell.execute(tokens.value());
    }

    if (getenv("NULLSH_IN_TERMINAL") == nullptr && cli->spawn_term)
    {
        // set to avoid infinite recursion
        setenv("NULLSH_IN_TERMINAL", "1", 1);

        std::string cmd = *cli->spawn_term; // NOLINT(bugprone-unchecked-optional-access)
        cmd += args[0];                     // path to nullsh

        for (const char* arg : args.subspan(1))
        {
            cmd += " ";
            cmd += arg;
        }

        return system(cmd.c_str());
    }

    shell.run();

    return 0;
}