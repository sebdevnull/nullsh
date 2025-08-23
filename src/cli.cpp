/**
 * @file cli.cpp
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <string_view>
#include <iostream>
#include <format>
#include "nullsh/cli.h"

using namespace nullsh::cli;
using namespace std::literals;

constexpr std::string_view helpmsg = "NullShell - embrace the void\n\n"
                                     "Usage:\n"
                                     "  nullsh            Start interactive shell\n"
                                     "  nullsh -c \"cmd\"  Execute a single command\n";

/**
 * @brief Parses arguments from command line
 * 
 * @param argc Number of arguments
 * @param argv Array of C-style string with the arguments
 * @return * std::expected<CLI, std::string> 
 */
std::expected<CLI, std::string> nullsh::cli::parse_cli(int argc, char **argv)
{
    CLI cli;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg = argv[i];
        if (arg == "-c"sv)
        {
            if (argc <= i + 1)
                return std::unexpected("Missing argument to -c");
            cli.one_shot = argv[++i];
        }
        else if (arg == "-h"sv || arg == "--help"sv)
        {
            std::cout << helpmsg;
            std::exit(0);
        }
        else
        {
            return std::unexpected(std::format("Unknown option: {}", arg));
        }
    }

    return cli;
}