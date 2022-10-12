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

#ifndef OMRE_UTILS_THREAD_UTILS_H
#define OMRE_UTILS_THREAD_UTILS_H

#if defined(WIN32)
// Include winsock2.h before including <windows.h> to maintain consistency with
// win32.h. To include win32.h directly, it must be broken out into its own
// build target.
#include <winsock2.h>
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

namespace utils {

#if defined(WIN32)
typedef DWORD ThreadId;
typedef DWORD ThreadRef;
#else
typedef pid_t ThreadId;
typedef pthread_t ThreadRef;
#endif

/**
 * Retrieve the ID of the current thread.
 */
ThreadId CurrentThreadId();

/**
 * Retrieves a reference to the current thread. On Windows, this is the same
 * as CurrentThreadId. On other platforms it's the pthread_t returned by
 * pthread_self().
 */
ThreadRef CurrentThreadRef();

/**
 * Compares two thread identifiers for equality.
 */
bool IsThreadRefEqual(const ThreadRef& a, const ThreadRef& b);

/**
 * Sets the current thread name.
 */
void SetCurrentThreadName(const char* name);

} // namespace utils

#endif //OMRE_UTILS_THREAD_UTILS_H
