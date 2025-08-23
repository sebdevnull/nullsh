/**
 * @file test_shell.cpp
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>
#include "nullsh/shell.h"

TEST(ShellTest, BasicRun)
{
    nullsh::NullShell sh;
    std::vector<std::string> args = {"echo", "hello", "void"};

    // Since run() enters an infinite loop, we cannot test it directly.
    // Instead, we can just ensure it doesn't throw an exception.
    int rc = -1;
    EXPECT_NO_THROW({
        // We will simulate a single command dispatch instead.
        rc = sh.dispatch(args);
    });

    EXPECT_EQ(rc, 0); // Assuming execute returns 0 for success
}

TEST(ShellTest, EmptyDispatch)
{
    nullsh::NullShell sh;
    std::vector<std::string> args; // empty args
    int rc = sh.dispatch(args);
    EXPECT_EQ(rc, 0); // Should handle empty command gracefully
}

TEST(ShellTest, SingleCommandDispatch)
{
    nullsh::NullShell sh;
    std::vector<std::string> args = {"echo", "hello", "void"};
    int rc = sh.dispatch(args);
    EXPECT_EQ(rc, 0); // Assuming execute returns 0 for success
}

TEST(ShellTest, MultipleCommandsDispatch)
{
    nullsh::NullShell sh;
    std::vector<std::string> args1 = {"echo", "first", "command"};
    std::vector<std::string> args2 = {"echo", "second", "command"};

    int rc1 = sh.dispatch(args1);
    int rc2 = sh.dispatch(args2);

    EXPECT_EQ(rc1, 0); // Assuming execute returns 0 for success
    EXPECT_EQ(rc2, 0); // Assuming execute returns 0 for success
}
