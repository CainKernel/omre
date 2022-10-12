/*
 * Copyright (C) 2022 The Open Media Rendering Engine Project
 * Copyright (C) 2022 cain.huang@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OMRE_UTILS_THREAD_H
#define OMRE_UTILS_THREAD_H

#include <utils/ThreadUtils.h>

#include <functional>
#include <optional>
#include <string>

namespace utils {

enum class ThreadPriority {
    kPriorityLow = 1,
    kPriorityNormal,
    kPriorityHigh,
    kRealtime,
};

struct ThreadAttributes {
    ThreadPriority priority = ThreadPriority::kPriorityNormal;

    ThreadAttributes& SetPriority(ThreadPriority priority_param) {
        this->priority = priority_param;
        return *this;
    }
};

class Thread final {
public:
#if defined(WIN32)
    using Handle = HANDLE;
#else
    using Handle = pthread_t;
#endif
    Thread() = default;
    Thread(Thread&& rhs) noexcept;
    Thread& operator=(Thread&& rhs) noexcept;
    virtual ~Thread();

    void finalize();

    /**
     * Returns true if default constructed, moved from, or finalize()ed.
     */
    bool empty() const { return !handle_.has_value(); }

    /**
     * Creates a started joinable thread which will be joined when the returned
     * Thread destructs or finalize() is called.
     */
    static Thread joinThread(
            std::function<void()> thread_function,
            std::string name,
            ThreadAttributes attributes = ThreadAttributes());

    /**
     * Creates a started detached thread. The caller has to use external synchronization
     * as nothing is provided by the Thread construct.
     */
    static Thread detachThread(
            std::function<void()> thread_function,
            std::string name,
            ThreadAttributes attributes = ThreadAttributes());

    /**
     * Returns the base platform thread handle of this thread.
     */
    std::optional<Handle> getHandle() const;

private:
    Thread(Handle handle, bool joinable);

    static Thread spawnThread(std::function<void()> thread_function,
            const std::string& name,
            ThreadAttributes attributes,
            bool joinable);

    std::optional<Handle> handle_;
    bool joinable_ = false;
};

} // namespace utils

#endif //OMRE_UTILS_THREAD_H
