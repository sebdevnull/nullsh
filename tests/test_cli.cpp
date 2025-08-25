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
    std::array args {"nullsh", "-c", "echo void"};
    auto cli = parse_cli(args);
    ASSERT_TRUE(cli.has_value());
    EXPECT_TRUE(cli->one_shot.has_value());
    EXPECT_EQ(cli->one_shot.value(), "echo void");
}

TEST(ParseCLI, MissingArg)
{
    std::array args {"nullsh", "-c"};
    auto cli = parse_cli(args);
    ASSERT_FALSE(cli.has_value());
    EXPECT_EQ(cli.error(), "Missing argument to -c");
}

TEST(ParseCLI, Help)
{
    std::array args {"nullsh", "-h"};
    // Since -h exits the program, we cannot test it directly.
    // Instead, we can just ensure it doesn't return an error.
    auto cli = parse_cli(args);
    ASSERT_FALSE(cli.has_value());
}

TEST(ParseCLI, NoArgs)
{
    std::array args {"nullsh"};
    auto cli = parse_cli(args);
    ASSERT_TRUE(cli.has_value());
    EXPECT_FALSE(cli->one_shot.has_value());
}

TEST(ParseCLI, UnknownOption)
{
    std::array args {"nullsh", "-x"};
    auto cli = parse_cli(args);
    ASSERT_FALSE(cli.has_value());
    EXPECT_EQ(cli.error(), "Unknown option: -x");
}