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

#include <utils/time.h>

#if defined(OMRE_POSIX)
#include <sys/time.h>
#include <ctime>
#endif

#if defined(OMRE_WIN)
#include <windows.h>
#include <mmsystem.h>
#include <sys/timeb.h>
#endif

namespace utils {

int64_t SystemTimeNanos() {
    int64_t ticks;
#if defined(OMRE_POSIX)
    struct timespec ts;
    // TODO(deadbeef): Do we need to handle the case when CLOCK_MONOTONIC is not
    // supported?
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ticks = kNumNanosecsPerSec * static_cast<int64_t>(ts.tv_sec) +
            static_cast<int64_t>(ts.tv_nsec);
#elif defined(WINUWP)
    ticks = WinUwpSystemTimeNanos();
#elif defined(OMRE_WIN)
    static volatile LONG last_timegettime = 0;
    static volatile int64_t num_wrap_timegettime = 0;
    volatile LONG* last_timegettime_ptr = &last_timegettime;
    DWORD now = timeGetTime();
    // Atomically update the last gotten time
    DWORD old = InterlockedExchange(last_timegettime_ptr, now);
    if (now < old) {
        // If now is earlier than old, there may have been a race between threads.
        // 0x0fffffff ~3.1 days, the code will not take that long to execute
        // so it must have been a wrap around.
        if (old > 0xf0000000 && now < 0x0fffffff) {
            num_wrap_timegettime++;
        }
    }
    ticks = now + (num_wrap_timegettime << 32);
    // TODO(deadbeef): Calculate with nanosecond precision. Otherwise, we're
    // just wasting a multiply and divide when doing Time() on Windows.
    ticks = ticks * kNumNanosecsPerMillisec;
#else
#error Unsupported platform.
#endif
    return ticks;
}

int64_t SystemTimeMillis() {
    return static_cast<int64_t>(SystemTimeNanos() / kNumNanosecsPerMillisec);
}

int64_t TimeNanos() {
    return SystemTimeNanos();
}

int64_t TimeMillis() {
    return TimeNanos() / kNumNanosecsPerMillisec;
}

int64_t TimeMicros() {
    return TimeNanos() / kNumNanosecsPerMicrosec;
}

} // namespace utils
