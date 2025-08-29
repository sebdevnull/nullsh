/**
 * @file test_executor.cpp
 * @brief Unit test for executor
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/executor.h"

using namespace nullsh::executor;

TEST(ExecutorTest, ExecExternalValidCommand)
{
    nullsh::command::Command cmd;
    cmd.name = "echo";
    cmd.args = {"Hello, World!"};

    auto res = exec_external(cmd);
    EXPECT_EQ(res.return_code, 0);
    EXPECT_EQ(res.stdout_data, "Hello, World!\n");
    EXPECT_EQ(res.stderr_data, "");
}

TEST(ExecutorTest, ExecExternalInvalidCommand)
{
    nullsh::command::Command cmd;
    cmd.name = "nonexistentcommand";
    cmd.args = {};

    auto res = exec_external(cmd);
    EXPECT_NE(res.return_code, 0); // should fail
    EXPECT_EQ(res.stdout_data, "");
    EXPECT_EQ(res.stderr_data, ""); // stderr may be empty if command not found
}

TEST(ExecutorTest, ExecExternalNoArgs)
{
    nullsh::command::Command cmd;
    cmd.name = "echo";
    cmd.args = {};

    auto res = exec_external(cmd);
    EXPECT_EQ(res.return_code, 0);
    EXPECT_EQ(res.stdout_data, "\n"); // echo with no args should print newline
    EXPECT_EQ(res.stderr_data, "");
}

TEST(ExecutorTest, ExecExternalWithError)
{
    nullsh::command::Command cmd;
    cmd.name = "ls";
    cmd.args = {"/nonexistentpath"};

    auto res = exec_external(cmd);
    EXPECT_NE(res.return_code, 0); // should fail
    EXPECT_EQ(res.stdout_data, "");
    EXPECT_NE(res.stderr_data, ""); // should have error message
}

TEST(ExecutorTest, ExecExternalEmptyCommand)
{
    nullsh::command::Command cmd;
    cmd.name = "";
    cmd.args = {};

    auto res = exec_external(cmd);
    EXPECT_EQ(res.return_code, 0); // should handle empty command gracefully
    EXPECT_EQ(res.stdout_data, "");
    EXPECT_EQ(res.stderr_data, "");
}

TEST(ExecutorTest, ApplyOperator)
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
        EXPECT_EQ(output, "0\n");
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