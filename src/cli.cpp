/**
 * @file cli.cpp
 * @brief CLI for NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/cli.h"

#include <format>
#include <iostream>
#include <string_view>
#include <vector>

#include "nullsh/util.h"
#include "nullsh/version.h"

namespace nullsh::cli
{
    using namespace std::literals;

    constexpr std::string_view HELP_MSG = "NullShell - embrace the void\n\n"
                                          "Usage:\n"
                                          "  nullsh            Start interactive shell\n"
                                          "  nullsh -c \"cmd\"  Execute a single command\n";

    namespace
    {
        const std::vector<std::string> TERMS = {
            "gnome-terminal", "konsole", "xfce4-terminal", "lxterminal", "alacritty", "xterm"};

        inline void spawn_terminal(CLI& cli)
        {
            for (const auto& term : TERMS)
            {
                if (nullsh::util::command_exists(term))
                {
                    std::string cmd = term;

                    if (term == "gnome-terminal" || term == "xfce4-terminal" ||
                        term == "lxterminal")
                    {
                        cmd += " -- ";
                        cli.spawn_term = cmd;
                        break;
                    }
                    if (term == "konsole" || term == "alacritty" || term == "xterm")
                    {
                        cmd += " -e ";
                        cli.spawn_term = cmd;
                        break;
                    }
                }
            }
        }
    } // namespace

    /**
     * @brief Parses arguments from command line
     *
     * @param argc Number of arguments
     * @param argv Array of C-style string with the arguments
     * @return * std::expected<CLI, std::string>
     */
    auto parse_cli(std::span<const char*> args) -> std::expected<CLI, std::string>
    {
        CLI cli {};

        for (size_t i = 1; i < args.size(); ++i)
        {
            std::string_view arg = args[i];
            if (arg == "-c"sv)
            {
                if (args.size() <= i + 1)
                {
                    return std::unexpected("Missing argument to -c");
                }
                cli.one_shot = args[++i];
            }
            else if (arg == "-h"sv || arg == "--help"sv)
            {
                std::cout << HELP_MSG;
                std::exit(0);
            }
            else if (arg == "-s"sv || arg == "--spawn"sv)
            {
                spawn_terminal(cli);
                if (!cli.spawn_term)
                {
                    return std::unexpected("Unable to find suitable terminal emulator wrapper");
                }
            }
            else if (arg == "-v"sv || arg == "--version"sv)
            {
                std::cout << "NullShell version " << VERSION_STR << " (commit " << GIT_COMMIT
                          << ")\n"
                          << "Build type: " << BUILD_TYPE << "\n"
                          << "Compiler: " << COMPILER_ID << " " << COMPILER_VERSION << "\n";
                std::exit(0);
            }
            else if (arg == "--build-info")
            {
                std::cout << "NullShell version " << VERSION_STR << " (commit " << GIT_COMMIT
                          << ", branch " << GIT_BRANCH << ", tag " << GIT_TAG << ")\n"
                          << "Build type:     " << BUILD_TYPE << "\n"
                          << "Compiler:       " << COMPILER_ID << " " << COMPILER_VERSION << "\n"
                          << "System:         " << SYSTEM_NAME << " (" << SYSTEM_PROCESSOR << ")\n"
                          << "CMake version:  " << CMAKE_VERSION << "\n"
                          << "Build timestamp:" << BUILD_TIMESTAMP << "\n";
                std::exit(0);
            }
            else
            {
                return std::unexpected(std::format("Unknown option: {}", arg));
            }
        }

        return cli;
    }
} // namespace nullsh::cli