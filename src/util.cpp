/**
 * @file utils.cpp
 * @brief Implementation of util.h
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "nullsh/util.h"
#include <cctype>

namespace nullsh::util
{
    /**
     * @brief Left trims whitespace from a string
     *
     * @param s String to trim
     */
    void ltrim(std::string &s)
    {
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
            ++start;

        s.erase(0, start);
    }

    /**
     * @brief Right trims whitespace from a string
     *
     * @param s String to trim
     */
    void rtrim(std::string &s)
    {
        size_t end = s.size();
        while (end > 0 && std::isspace(static_cast<unsigned char>(s[end - 1])))
            --end;

        s.erase(end);
    }

    /**
     * @brief Trims whitespace from both ends of a string
     *
     * @param s String to trim
     */
    void trim(std::string &s)
    {
        rtrim(s);
        ltrim(s);
    }

    /**
     * @brief Tokenizes a command line into individual arguments
     *
     * @param line Command line to tokenize
     * @return std::expected<std::vector<std::string>, std::string>
     */
    std::expected<std::vector<std::string>, std::string> tokenize(std::string_view line)
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
            char c = line[i];

            // escape next char
            if (c == '\\')
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
            else if (c == '\'' && !in_double_quote)
            {
                in_single_quote = !in_single_quote;
            }
            else if (c == '\"' && !in_single_quote)
            {
                in_double_quote = !in_double_quote;
            }
            else if (std::isspace(static_cast<unsigned char>(c)) && !in_single_quote && !in_double_quote)
            {
                push_token();
            }
            else
            {
                cur.push_back(c);
            }
        }

        if (in_single_quote || in_double_quote)
        {
            return std::unexpected("Mismatched quotes in command line");
        }

        push_token();
        return tokens;
    }
} // namespace nullsh::util