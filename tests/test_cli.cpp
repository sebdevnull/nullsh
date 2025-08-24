/**
 * @file test_cli.cpp
 * @brief Tests for nullsh::cli
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>
#include "nullsh/cli.h"

using namespace nullsh::cli;

TEST(ParseCLI, OneShot)
{
    int argc = 3;
    const char *argv[] = {"nullsh", "-c", "echo void"};
    auto cli = parse_cli(argc, const_cast<char **>(argv));
    ASSERT_TRUE(cli.has_value());
    EXPECT_TRUE(cli->one_shot.has_value());
    EXPECT_EQ(cli->one_shot.value(), "echo void");    
}

TEST(ParseCLI, MissingArg)
{
    int argc = 2;
    const char *argv[] = {"nullsh", "-c"};
    auto cli = parse_cli(argc, const_cast<char **>(argv));
    ASSERT_FALSE(cli.has_value());
    EXPECT_EQ(cli.error(), "Missing argument to -c");
}

TEST(ParseCLI, Help)
{
    int argc = 2;
    const char *argv[] = {"nullsh", "-h"};
    // Since -h exits the program, we cannot test it directly.
    // Instead, we can just ensure it doesn't return an error.
    auto cli = parse_cli(argc, const_cast<char **>(argv));
    ASSERT_FALSE(cli.has_value());
}

TEST(ParseCLI, NoArgs)
{
    int argc = 1;
    const char *argv[] = {"nullsh"};
    auto cli = parse_cli(argc, const_cast<char **>(argv));
    ASSERT_TRUE(cli.has_value());
    EXPECT_FALSE(cli->one_shot.has_value());
}

TEST(ParseCLI, UnknownOption)
{
    int argc = 2;
    const char *argv[] = {"nullsh", "-x"};
    auto cli = parse_cli(argc, const_cast<char **>(argv));
    ASSERT_FALSE(cli.has_value());
    EXPECT_EQ(cli.error(), "Unknown option: -x");
}