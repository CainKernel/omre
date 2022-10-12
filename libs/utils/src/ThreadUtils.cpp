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

#include <utils/ThreadUtils.h>

#if defined(WIN32)
#include <utils/arraysize.h>

// The SetThreadDescription API was brought in version 1607 of Windows 10.
// For compatibility with various versions of winuser and avoid clashing with
// a potentially defined type, we use the OMRE_ prefix.
typedef HRESULT(WINAPI* SetThreadDescription)(HANDLE hThread,
                                              PCWSTR lpThreadDescription);
#else

#include <sys/prctl.h>
#include <sys/syscall.h>

#endif

namespace utils {

ThreadId CurrentThreadId() {
#if defined(WIN32)
    return GetCurrentThreadId();
#elif defined(APPLE) || defined(__APPLE__)
    return pthread_mach_thread_np(pthread_self());
#elif defined(ANDROID) || defined(__ANDROID__)
    return gettid();
#elif defined(__LINUX__)
    return syscall(__NR_gettid);
#elif defined(__EMSCRIPTEN__)
    return static_cast<ThreadId>(pthread_self());
#else
    // Default implementation for nacl and solaris.
    return reinterpret_cast<ThreadId>(pthread_self());
#endif
}

ThreadRef CurrentThreadRef() {
#if defined(WIN32)
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

bool IsThreadRefEqual(ThreadRef const& a, ThreadRef const& b) {
#if defined(WIN32)
    return a == b;
#else
    return pthread_equal(a, b);
#endif
}

void SetCurrentThreadName(const char* name) {
#if defined(WIN32)
    // The SetThreadDescription API works even if no debugger is attached.
    // The names set with this API also show up in ETW traces. Very handy.
    static auto set_thread_description_func =
        reinterpret_cast<SetThreadDescription>(::GetProcAddress(
            ::GetModuleHandleA("Kernel32.dll"), "SetThreadDescription"));
    if (set_thread_description_func) {
        // Convert from ASCII to UTF-16.
        wchar_t wide_thread_name[64];
        for (size_t i = 0; i < arraysize(wide_thread_name) - 1; ++i) {
            wide_thread_name[i] = name[i];
            if (wide_thread_name[i] == L'\0')
                break;
        }
        // Guarantee null-termination.
        wide_thread_name[arraysize(wide_thread_name) - 1] = L'\0';
        set_thread_description_func(::GetCurrentThread(), wide_thread_name);
    }

    // For details see:
    // https://docs.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code
#pragma pack(push, 8)
    struct {
        DWORD dwType;
        LPCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags;
    } threadname_info = {0x1000, name, static_cast<DWORD>(-1), 0};
#pragma pack(pop)

#pragma warning(push)
#pragma warning(disable : 6320 6322)
    __try {
        ::RaiseException(0x406D1388, 0, sizeof(threadname_info) / sizeof(ULONG_PTR),
                         reinterpret_cast<ULONG_PTR*>(&threadname_info));
    } __except (EXCEPTION_EXECUTE_HANDLER) {  // NOLINT
    }
#pragma warning(pop)
#elif defined(APPLE) || defined(__APPLE__)
    pthread_setname_np(name);
#elif defined(LINUX) || defined(ANDROID)
    prctl(PR_SET_NAME, reinterpret_cast<unsigned long>(name));  // NOLINT
#endif
}


} // namespace utils
