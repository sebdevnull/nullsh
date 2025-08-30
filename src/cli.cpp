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

    constexpr std::string_view NULLSH_LOGO = R"(  _   _       _ _  _____ _          _ _ 
 | \ | |     | | |/ ____| |        | | |
 |  \| |_   _| | | (___ | |__   ___| | |
 | . ` | | | | | |\___ \| '_ \ / _ \ | |
 | |\  | |_| | | |____) | | | |  __/ | |
 |_| \_|\__,_|_|_|_____/|_| |_|\___|_|_|
)";

    constexpr std::string_view HELP_BODY = R"(The intentional shell for a noisy world.
Embrace the void. Execute with purpose.

Usage:
  nullsh [OPTIONS]

Options:
  -h, --help        Show this help message and exit
  -v, --version     Show version information and exit
      --build-info  Show build info (compiler, flags, etc.)
  -c, --command     Execute a single command and exit
  -s, --spawn       Launch nullsh in a new terminal window

Operators:
  !       Force output: print stdout and stderr
  ?       Silent run: suppress all output, even on failure
  $?      Return code: print numeric exit code of last command
  $$?     Verbose return code: exit code with success/failure note

Built-in Commands:
  cd [dir]      Change current directory
  pwd           Print current working directory
  echo [args]   Print arguments (silent without '!')
  exit [code]   Exit the shell

Examples:
  nullsh                  Start interactive session
  nullsh -c 'ls /tmp !'   Execute command and exit
  nullsh --spawn          Launch in a new terminal window

Notes:
- Commands succeed silently by default; errors are shown.
- Operators modify behavior or display return codes.
)";

    constexpr std::string_view LICENSE_SHORT =
        "License GPLv3: GNU General Public License version 3";
    constexpr std::string_view LICENSE_DETAILS =
        "For detailed license information, see LICENSE (GNU GPLv3) "
        "<https://www.gnu.org/licenses/gpl-3.0.html>";

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
                std::cout << NULLSH_LOGO << "\n"
                          << "NullShell (nullsh) v" << VERSION_STR << "\n"
                          << LICENSE_SHORT << "\n"
                          << "\n"
                          << HELP_BODY << "\n"
                          << LICENSE_DETAILS << "\n";
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
                          << LICENSE_SHORT << "\n"
                          << "Build type: " << BUILD_TYPE << "\n"
                          << "Compiler: " << COMPILER_ID << " " << COMPILER_VERSION << "\n";
                std::exit(0);
            }
            else if (arg == "--build-info")
            {
                std::cout << "NullShell version " << VERSION_STR << " (commit " << GIT_COMMIT
                          << ", branch " << GIT_BRANCH << ", tag " << GIT_TAG << ")\n"
                          << LICENSE_SHORT << "\n"
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