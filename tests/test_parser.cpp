/**
 * @file test_parser.cpp
 * @brief Unit tests for parser
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/command.h"
#include "nullsh/parser.h"

TEST(ParserTest, ParseOperator)
{
    using namespace nullsh::parser;
    using namespace nullsh::command;

    EXPECT_EQ(parse_operator("!"), Op::ForceOutput);
    EXPECT_EQ(parse_operator("?"), Op::DiscardOutput);
    EXPECT_EQ(parse_operator("$?"), Op::PrintRC);
    EXPECT_EQ(parse_operator("$??"), Op::PrintRCHuman);
    EXPECT_EQ(parse_operator("unknown"), Op::None);
}

TEST(ParserTest, ParseCommand)
{
    using namespace nullsh::parser;
    using namespace nullsh::command;

    {
        std::vector<std::string> tokens = {"echo", "Hello", "World", "!"};
        Command cmd = parse_command(tokens);
        EXPECT_EQ(cmd.args, std::vector<std::string>({"echo", "Hello", "World"}));
        EXPECT_EQ(cmd.ops, std::vector<Op>({Op::ForceOutput}));
    }
    {
        std::vector<std::string> tokens = {"ls", "-l", "?", "$?"};
        Command cmd = parse_command(tokens);
        EXPECT_EQ(cmd.args, std::vector<std::string>({"ls", "-l"}));
        EXPECT_EQ(cmd.ops, std::vector<Op>({Op::DiscardOutput, Op::PrintRC}));
    }
    {
        std::vector<std::string> tokens = {"cat", "file.txt"};
        Command cmd = parse_command(tokens);
        EXPECT_EQ(cmd.args, std::vector<std::string>({"cat", "file.txt"}));
        EXPECT_TRUE(cmd.ops.empty());
    }
}
