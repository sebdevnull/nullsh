/**
 * @file test_result_capturer.cpp
 * @brief Unit tests for command result capturer
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/command.h"
#include "nullsh/result_capturer.h"

using namespace nullsh;

// class ResultCapturerTest : public ::testing::Test
// {
//   protected:
//     void SetUp() override {
//         res = std::make_unique<command::CommandResult>();
//         capturer = std::make_unique<io::CommandResultCapturer>(*res);
//     }
//     void TearDown() override {
//     }

//     std::unique_ptr<command::CommandResult> res;
//     std::unique_ptr<io::CommandResultCapturer> capturer;
// };

TEST(ResultCapturerTest, CaptureFlow)
{
    command::CommandResult res {};
    io::CommandResultCapturer capturer {res};

    EXPECT_NO_THROW(capturer.init_pipes());

    pid_t pid = fork();
    if (pid < 0)
    {
        FAIL() << "Fork failed";
    }
    if (pid == 0)
    {
        // Child process
        EXPECT_NO_THROW(capturer.prepare_child());
        std::cout << "Hello, World!" << '\n';
        std::cerr << "Error message" << '\n';
        _exit(0);
    }
    else
    {
        // Parent process
        EXPECT_NO_THROW(capturer.capture_parent(pid));
        EXPECT_EQ(res.return_code, 0);
        EXPECT_EQ(res.stdout_data, "Hello, World!\n");
        EXPECT_EQ(res.stderr_data, "Error message\n");
    }
}
