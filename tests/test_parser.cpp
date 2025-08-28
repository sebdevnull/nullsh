/**
 * @file test_parser.cpp
 * @brief Unit tests for parser
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/command.h"
#include "nullsh/parser.h"

using namespace nullsh::parser;
using namespace nullsh::command;

TEST(ParserTest, ParseOperator)
{

    EXPECT_EQ(parse_operator("!"), Op::ForceOutput);
    EXPECT_EQ(parse_operator("?"), Op::DiscardOutput);
    EXPECT_EQ(parse_operator("$?"), Op::PrintRC);
    EXPECT_EQ(parse_operator("$$?"), Op::PrintRCHuman);
    EXPECT_EQ(parse_operator("unknown"), Op::None);
}

TEST(ParserTest, ParseCommand)
{
    std::vector<std::string> tokens = {"cat", "file.txt"};
    auto cmd = make_command(tokens);
    ASSERT_TRUE(cmd.has_value());
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    EXPECT_EQ(cmd->name, "cat");
    EXPECT_EQ(cmd->args, std::vector<std::string>({"file.txt"}));
    EXPECT_TRUE(cmd->ops.empty());
    // NOLINTEND(bugprone-unchecked-optional-access)
}

TEST(ParserTest, ParseCommandSingleOp)
{
    std::vector<std::string> tokens = {"echo", "Hello", "World", "!"};
    auto cmd = make_command(tokens);
    ASSERT_TRUE(cmd.has_value());
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    EXPECT_EQ(cmd->name, "echo");
    EXPECT_EQ(cmd->args, std::vector<std::string>({"Hello", "World"}));
    EXPECT_EQ(cmd->ops, std::vector<Op>({Op::ForceOutput}));
    // NOLINTEND(bugprone-unchecked-optional-access)
}

TEST(ParserTest, ParseCommandMultipleOps)
{
    std::vector<std::string> tokens = {"ls", "-l", "?", "$?"};
    auto cmd = make_command(tokens);
    ASSERT_TRUE(cmd.has_value());
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    EXPECT_EQ(cmd->name, "ls");
    EXPECT_EQ(cmd->args, std::vector<std::string>({"-l"}));
    EXPECT_EQ(cmd->ops, std::vector<Op>({Op::DiscardOutput, Op::PrintRC}));
    // NOLINTEND(bugprone-unchecked-optional-access)
}
