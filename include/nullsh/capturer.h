/**
 * @file capturer.h
 * @brief Interface for capturing input/output from child processes.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <sys/types.h>

namespace nullsh::io
{
    class IOCapturer
    {
      public:
        virtual ~IOCapturer() = default;
        IOCapturer() = default;
        IOCapturer(const IOCapturer&) = delete;
        IOCapturer& operator=(const IOCapturer&) = delete;
        IOCapturer(IOCapturer &&) = delete;
        IOCapturer& operator=(IOCapturer&&) = delete;

        virtual void prepare_child() = 0;
        virtual void capture_parent(pid_t pid) = 0;
    };
} // namespace nullsh::io