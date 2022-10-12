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

#ifndef OMRE_UTILS_TIME_H
#define OMRE_UTILS_TIME_H

#include <cstdint>
#include <ctime>

#include <utils/export.h>

namespace utils {

static const int64_t kNumMillisecsPerSec = INT64_C(1000);
static const int64_t kNumMicrosecsPerSec = INT64_C(1000000);
static const int64_t kNumNanosecsPerSec = INT64_C(1000000000);

static const int64_t kNumMicrosecsPerMillisec =
        kNumMicrosecsPerSec / kNumMillisecsPerSec;
static const int64_t kNumNanosecsPerMillisec =
        kNumNanosecsPerSec / kNumMillisecsPerSec;
static const int64_t kNumNanosecsPerMicrosec =
        kNumNanosecsPerSec / kNumMicrosecsPerSec;

/**
 * Returns the actual system time, even if a clock is set for testing.
 * Useful for timeouts while using a test clock, or for logging.
 */
int64_t SystemTimeMillis();

/**
 * Returns the current time in milliseconds in 64 bits.
 */
OMRE_EXPORT int64_t TimeMillis();

/**
 * Returns the current time in microseconds.
 */
OMRE_EXPORT int64_t TimeMicros();

/**
 * Returns the current time in nanoseconds.
 */
OMRE_EXPORT int64_t TimeNanos();

} // namespace utils

#endif //OMRE_UTILS_TIME_H
