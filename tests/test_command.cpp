/**
 * @file test_command.cpp
 * @brief Unit tests for command handling
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/command.h"

TEST(CommandTest, ApplyOperator)
{
    using namespace nullsh::command;

    CommandResult res;
    res.return_code = 0;
    res.stdout_data = "Hello, World!\n";
    res.stderr_data = "Error message\n";

    // Test ForceOutput
    {
        CommandResult temp_res = res;
        apply_operator(Op::ForceOutput, temp_res);
        EXPECT_EQ(temp_res.stdout_data, "Hello, World!\n");
        EXPECT_EQ(temp_res.stderr_data, "Error message\n");
    }

    // Test DiscardOutput
    {
        CommandResult temp_res = res;
        apply_operator(Op::DiscardOutput, temp_res);
        EXPECT_EQ(temp_res.stdout_data, "");
        EXPECT_EQ(temp_res.stderr_data, "");
    }

    // Test PrintRC
    {
        CommandResult temp_res = res;
        testing::internal::CaptureStdout();
        apply_operator(Op::PrintRC, temp_res);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, "0");
    }

    // Test PrintRCHuman
    {
        CommandResult temp_res = res;
        testing::internal::CaptureStdout();
        apply_operator(Op::PrintRCHuman, temp_res);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, "0 (success)\n");
    }

    // Test None (default)
    {
        // should discard stdout, keep stderr
        CommandResult temp_res = res;
        testing::internal::CaptureStderr();
        apply_operator(Op::None, temp_res);
        std::string err_output = testing::internal::GetCapturedStderr();
        EXPECT_EQ(temp_res.stdout_data, "");
        EXPECT_EQ(err_output, "Error message\n");
    }
}