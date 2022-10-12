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

#include <utils/Thread.h>

#include <algorithm>
#include <memory>
#include <optional>

#if !defined(WIN32)

#include <sched.h>

#endif

namespace utils {

namespace details {

#if defined(WIN32)

int Win32PriorityFromThreadPriority(ThreadPriority priority) {
  switch (priority) {
    case ThreadPriority::kPriorityLow:
      return THREAD_PRIORITY_BELOW_NORMAL;
    case ThreadPriority::kPriorityNormal:
      return THREAD_PRIORITY_NORMAL;
    case ThreadPriority::kPriorityHigh:
      return THREAD_PRIORITY_ABOVE_NORMAL;
    case ThreadPriority::kRealtime:
      return THREAD_PRIORITY_TIME_CRITICAL;
  }
}

#endif

bool SetPriority(ThreadPriority priority) {
#if defined(WIN32)
    return SetThreadPriority(GetCurrentThread(),
                             Win32PriorityFromThreadPriority(priority)) != FALSE;
#else
    const int policy = SCHED_FIFO;
    const int min_prio = sched_get_priority_min(policy);
    const int max_prio = sched_get_priority_max(policy);
    if (min_prio == -1 || max_prio == -1) {
        return false;
    }

    if (max_prio - min_prio <= 2) {
        return false;
    }

    // Convert omre priority to system priorities:
    sched_param param{};
    const int top_prio = max_prio - 1;
    const int low_prio = min_prio + 1;
    switch (priority) {
        case ThreadPriority::kPriorityLow: {
            param.sched_priority = low_prio;
            break;
        }
        case ThreadPriority::kPriorityNormal: {
            // The -1 ensures that the kHighPriority is always greater or equal to
            // kNormalPriority.
            param.sched_priority = (low_prio + top_prio - 1) / 2;
            break;
        }
        case ThreadPriority::kPriorityHigh: {
            param.sched_priority = std::max(top_prio - 2, low_prio);
            break;
        }
        case ThreadPriority::kRealtime: {
            param.sched_priority = top_prio;
            break;
        }
        default: {
            break;
        }
    }
    return pthread_setschedparam(pthread_self(), policy, &param) == 0;
#endif  // defined(WIN32)
}

#if defined(WIN32)

DWORD WINAPI RunPlatformThread(void* param) {
  // The GetLastError() function only returns valid results when it is called
  // after a Win32 API function that returns a "failed" result. A crash dump
  // contains the result from GetLastError() and to make sure it does not
  // falsely report a Windows error we call SetLastError here.
  ::SetLastError(ERROR_SUCCESS);
  auto function = static_cast<std::function<void()>*>(param);
  (*function)();
  delete function;
  return 0;
}

#else

void* RunPlatformThread(void* param) {
    auto function = static_cast<std::function<void()>*>(param);
    (*function)();
    delete function;
    return nullptr;
}

#endif  // defined(WIN32)

} // namespace details

Thread::Thread(Handle handle, bool joinable) : handle_(handle), joinable_(joinable) {}

Thread::Thread(Thread&& rhs) noexcept: handle_(rhs.handle_), joinable_(rhs.joinable_) {
    rhs.handle_ = std::nullopt;
}

Thread& Thread::operator=(Thread&& rhs) noexcept {
    finalize();
    handle_ = rhs.handle_;
    joinable_ = rhs.joinable_;
    rhs.handle_ = std::nullopt;
    return *this;
}

Thread::~Thread() {
    finalize();
}

Thread Thread::joinThread(std::function<void()> thread_function, std::string name,
        ThreadAttributes attributes) {
    return spawnThread(std::move(thread_function), name, attributes, true);
}

Thread Thread::detachThread(std::function<void()> thread_function, std::string name,
        ThreadAttributes attributes) {
    return spawnThread(std::move(thread_function), name, attributes, false);
}

std::optional<Thread::Handle> Thread::getHandle() const {
    return handle_;
}

void Thread::finalize() {
    if (!handle_.has_value()) {
        return;
    }

#if defined(WIN32)
    if (joinable_) {
        WaitForSingleObject(*handle_, INFINITE);
    }
    CloseHandle(*handle_);
#else
    if (joinable_) {
        pthread_join(*handle_, nullptr);
    }
#endif
    handle_ = std::nullopt;
}

Thread Thread::spawnThread(std::function<void()> thread_function, const std::string& name,
        ThreadAttributes attributes, bool joinable) {
    auto start_thread_function_ptr =
            new std::function<void()>([thread_function = std::move(thread_function),
                    name, attributes] {
                SetCurrentThreadName(name.c_str());
                details::SetPriority(attributes.priority);
                thread_function();
            });

#if defined(WIN32)
    // See bug 2902 for background on STACK_SIZE_PARAM_IS_A_RESERVATION.
    // Set the reserved stack stack size to 1M, which is the default on Windows
    // and Linux.
    DWORD thread_id = 0;
    Thread::Handle handle = ::CreateThread(
            nullptr, 1024 * 1024, &details::RunPlatformThread, start_thread_function_ptr,
            STACK_SIZE_PARAM_IS_A_RESERVATION, &thread_id);
#else

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // Set the stack stack size to 1M.
    pthread_attr_setstacksize(&attr, 1024 * 1024);
    pthread_attr_setdetachstate(
            &attr, joinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);
    Thread::Handle handle;
    pthread_create(&handle, &attr, &details::RunPlatformThread, start_thread_function_ptr);
    pthread_attr_destroy(&attr);

#endif // defined(WIN32)

    return { handle, joinable };
}

} // namespace utils
