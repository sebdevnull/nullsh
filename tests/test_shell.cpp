/**
 * @file test_shell.cpp
 * @brief Tests for nullsh::NullShell
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "nullsh/shell.h"

using namespace nullsh::shell;

TEST(ShellTest, BasicRun)
{
    NullShell shell;
    std::vector<std::string> args = {"echo", "hello", "void"};

    // Since run() enters an infinite loop, we cannot test it directly.
    // Instead, we can just ensure it doesn't throw an exception.
    int rc = -1;
    EXPECT_NO_THROW({
        // We will simulate a single command dispatch instead.
        rc = shell.execute(args);
    });

    EXPECT_EQ(rc, 0); // Assuming execute returns 0 for success
}

TEST(ShellTest, EmptyExecute)
{
    NullShell shell;
    std::vector<std::string> args; // empty args
    int rc = shell.execute(args);
    EXPECT_EQ(rc, 0); // Should handle empty command gracefully
}

TEST(ShellTest, SingleCommandExecute)
{
    NullShell shell;
    std::vector<std::string> args = {"echo", "hello", "void"};
    int rc = shell.execute(args);
    EXPECT_EQ(rc, 0); // Assuming execute returns 0 for success
}

TEST(ShellTest, MultipleCommandsExecute)
{
    NullShell shell;
    std::vector<std::string> args1 = {"echo", "first", "command"};
    std::vector<std::string> args2 = {"echo", "second", "command"};

    int rc1 = shell.execute(args1);
    int rc2 = shell.execute(args2);

    EXPECT_EQ(rc1, 0); // Assuming execute returns 0 for success
    EXPECT_EQ(rc2, 0); // Assuming execute returns 0 for success
}
