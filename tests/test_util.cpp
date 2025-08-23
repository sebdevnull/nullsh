/**
 * @file test_util.cpp
 * @brief Tests for nullsh::util
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>
#include "nullsh/util.h"

TEST(LTrimTest, RemovesSpaces)
{
    std::string s = "     test";
    nullsh::util::ltrim(s);
    EXPECT_EQ(s, "test");
}

TEST(RTrimTest, RemovesSpaces)
{
    std::string s = "test    ";
    nullsh::util::rtrim(s);
    EXPECT_EQ(s, "test");
}

TEST(TrimTest, RemovesSpaces)
{
    std::string s = "     test      ";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "test");
}

TEST(TrimTest, KeepsInternalSpaces)
{
    std::string s = "     hello void    ";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "hello void");
}

TEST(TrimTest, EmptyString)
{
    std::string s = "";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "");
}

TEST(TrimTest, AllSpaces)
{
    std::string s = "       ";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "");
}

TEST(TrimTest, NoSpaces)
{
    std::string s = "void";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "void");
}

TEST(TrimTest, MixedWhitespace)
{
    std::string s = "\t  \n  void \t \n ";
    nullsh::util::trim(s);
    EXPECT_EQ(s, "void");
}

TEST(TokenizeTest, SimpleCommand)
{
    auto result = nullsh::util::tokenize("echo hello void");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
    EXPECT_EQ((*result)[0], "echo");
    EXPECT_EQ((*result)[1], "hello");
    EXPECT_EQ((*result)[2], "void");
}

TEST(TokenizeTest, QuotedArguments)
{
    auto result = nullsh::util::tokenize("echo 'hello void' \"and universe\"");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
    EXPECT_EQ((*result)[0], "echo");
    EXPECT_EQ((*result)[1], "hello void");
    EXPECT_EQ((*result)[2], "and universe");
}

TEST(TokenizeTest, MixedQuotes)
{
    auto result = nullsh::util::tokenize("cmd \"arg with 'single quotes'\" 'arg with \"double quotes\"'");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
    EXPECT_EQ((*result)[0], "cmd");
    EXPECT_EQ((*result)[1], "arg with 'single quotes'");
    EXPECT_EQ((*result)[2], "arg with \"double quotes\"");
}

TEST(TokenizeTest, EscapedQuotes)
{
    auto result = nullsh::util::tokenize("echo \\\"escaped\\\" \\'quotes\\'");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
    EXPECT_EQ((*result)[0], "echo");
    EXPECT_EQ((*result)[1], "\"escaped\"");
    EXPECT_EQ((*result)[2], "'quotes'");
}

TEST(TokenizeTest, MismatchedQuotes)
{
    auto result = nullsh::util::tokenize("echo 'mismatched quotes");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Mismatched quotes in command line");
}

TEST(TokenizeTest, EmptyInput)
{
    auto result = nullsh::util::tokenize("");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 0);
}

TEST(TokenizeTest, OnlySpaces)
{
    auto result = nullsh::util::tokenize("     ");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 0);
}

TEST(TokenizeTest, ComplexCommand)
{
    auto result = nullsh::util::tokenize("  cmd   arg1  'arg two'  \"arg three\"  ");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4);
    EXPECT_EQ((*result)[0], "cmd");
    EXPECT_EQ((*result)[1], "arg1");
    EXPECT_EQ((*result)[2], "arg two");
    EXPECT_EQ((*result)[3], "arg three");
}

TEST(TokenizeTest, TrailingBackslash)
{
    auto result = nullsh::util::tokenize("echo hello\\");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2);
    EXPECT_EQ((*result)[0], "echo");
    EXPECT_EQ((*result)[1], "hello\\");
}

TEST(TokenizeTest, EscapedSpaces)
{
    auto result = nullsh::util::tokenize("echo hello\\ world");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2);
    EXPECT_EQ((*result)[0], "echo");
    EXPECT_EQ((*result)[1], "hello world");
}

TEST(CommandExistsTest, ExistingCommand)
{
    EXPECT_TRUE(nullsh::util::command_exists("ls")); // Assuming 'ls' exists on the system
}

TEST(CommandExistsTest, NonExistingCommand)
{
    EXPECT_FALSE(nullsh::util::command_exists("some_nonexistent_command_12345"));
}

TEST(CommandExistsTest, EmptyCommand)
{
    EXPECT_FALSE(nullsh::util::command_exists(""));
}

TEST(CommandExistsTest, NullCommand)
{
    EXPECT_FALSE(nullsh::util::command_exists("   "));
}

TEST(CommandExistsTest, CommandWithPath)
{
    EXPECT_FALSE(nullsh::util::command_exists("/bin/ls")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithArguments)
{
    EXPECT_FALSE(nullsh::util::command_exists("ls -l")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithSpecialChars)
{
    EXPECT_FALSE(nullsh::util::command_exists("ls; echo void")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithSpaces)
{
    EXPECT_FALSE(nullsh::util::command_exists("ls ")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithTabs)
{
    EXPECT_FALSE(nullsh::util::command_exists("ls\t")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithNewline)
{
    EXPECT_FALSE(nullsh::util::command_exists("ls\n")); // Should return false as we check only command names
}

TEST(CommandExistsTest, CommandWithMixedWhitespace)
{
    EXPECT_FALSE(nullsh::util::command_exists("  ls  ")); // Should return false as we check only command names
}

TEST(CommandExistsTest, VeryLongCommand)
{
    std::string long_cmd(1000, 'a'); // Command name with 1000 'a' characters
    EXPECT_FALSE(nullsh::util::command_exists(long_cmd));
}