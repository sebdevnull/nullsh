/**
 * @file utils.cpp
 * @brief Common utility components
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/util.h"

#include <unistd.h>

#include <cctype>
#include <filesystem>
#include <format>
#include <optional>
#include <sstream>
#include <system_error>

namespace nullsh::util
{
    /**
     * @brief Left trims whitespace from a string
     *
     * @param s String to trim
     */
    void ltrim(std::string& str)
    {
        size_t start = 0;
        while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])) != 0)
        {
            ++start;
        }

        str.erase(0, start);
    }

    /**
     * @brief Right trims whitespace from a string
     *
     * @param s String to trim
     */
    void rtrim(std::string& str)
    {
        size_t end = str.size();
        while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1])) != 0)
        {
            --end;
        }

        str.erase(end);
    }

    /**
     * @brief Trims whitespace from both ends of a string
     *
     * @param s String to trim
     */
    void trim(std::string& str)
    {
        rtrim(str);
        ltrim(str);
    }

    /**
     * @brief Ensures a string ends with a newline character
     *
     * @param str String to modify
     */
    void newline(std::string& str)
    {
        if (!str.empty() && str.back() != '\n')
        {
            str.push_back('\n');
        }
    }

    /**
     * @brief Tokenizes a command line into individual arguments
     *
     * @param line Command line to tokenize
     * @return std::expected<std::vector<std::string>, std::string>
     */
    auto tokenize(std::string_view line) -> std::expected<std::vector<std::string>, std::string>
    {
        std::vector<std::string> tokens;
        std::string cur;
        bool in_single_quote = false;
        bool in_double_quote = false;

        auto push_token = [&]()
        {
            if (!cur.empty())
            {
                tokens.emplace_back(std::move(cur));
                cur.clear();
            }
        };

        for (size_t i = 0; i < line.size(); ++i)
        {
            char cur_c = line[i];

            // escape next char
            if (cur_c == '\\')
            {
                if (i + 1 < line.size())
                {
                    cur.push_back(line[++i]);
                }
                else
                {
                    // trailing backslash, treat as literal
                    cur.push_back('\\');
                }
            }
            else if (cur_c == '\'' && !in_double_quote)
            {
                in_single_quote = !in_single_quote;
            }
            else if (cur_c == '\"' && !in_single_quote)
            {
                in_double_quote = !in_double_quote;
            }
            else if (std::isspace(static_cast<unsigned char>(cur_c)) != 0 && !in_single_quote &&
                     !in_double_quote)
            {
                push_token();
            }
            else
            {
                cur.push_back(cur_c);
            }
        }

        if (in_single_quote || in_double_quote)
        {
            return std::unexpected("Mismatched quotes in command line");
        }

        push_token();
        return tokens;
    }

    /**
     * @brief Checks if a command exists in the system PATH
     *
     * @param cmd Command to check
     * @return true if command exists, false otherwise
     */
    bool command_exists(const std::string& cmd)
    {
        if (cmd.empty())
        {
            return false;
        }

        const char* path_env = std::getenv("PATH");
        if (path_env == nullptr)
        {
            return false;
        }

        std::string path(path_env);
        std::stringstream sstr(path);
        std::string dir;

        while (std::getline(sstr, dir, ':'))
        {
            std::string full_path = std::format("{}/{}", dir, cmd);
            if (access(full_path.c_str(), X_OK) == 0)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Gets the value of an environment variable
     *
     * @param name
     * @return std::optional<std::string>
     */
    auto get_env_var(const std::string& name) -> std::optional<std::string>
    {
        if (const char* value = std::getenv(name.c_str()); value != nullptr)
        {
            return std::string(value);
        }
        return std::nullopt;
    }

    /**
     * @brief Expands a user path, replacing ~ with the user's home directory
     *
     * @param path
     * @return std::filesystem::path
     */
    auto expand_user_path(const std::string& path) -> std::filesystem::path
    {
        if (path.empty() || path[0] != '~')
        {
            return {path};
        }

        auto home = get_env_var("HOME");
        if (!home.has_value())
        {
            return {path};
        }

        if (path.size() == 1)
        {
            return {*home};
        }

        if (path[1] == '/')
        {
            return std::filesystem::path(*home) / path.substr(2);
        }

        return {path};
    }

    /**
     * @brief Resolves a directory path to an absolute path, checking existence and type
     *
     * @param path Input path to resolve
     * @param resolved_path Output parameter for the resolved absolute path
     * @return std::error_code indicating success or failure reason
     */
    std::error_code resolve_directory(const std::filesystem::path& path,
                                      std::filesystem::path& resolved_path)
    {
        std::error_code ec;
        resolved_path = std::filesystem::absolute(path, ec);
        if (ec)
        {
            return ec;
        }

        if (!std::filesystem::exists(resolved_path, ec))
        {
            return ec ? ec : std::make_error_code(std::errc::no_such_file_or_directory);
        }

        if (!std::filesystem::is_directory(resolved_path, ec))
        {
            return ec ? ec : std::make_error_code(std::errc::not_a_directory);
        }

        return {};
    }

} // namespace nullsh::util