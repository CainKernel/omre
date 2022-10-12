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

#include <utils/Log.h>

#include <string>
#include <utils/compiler.h>

#ifdef ANDROID

#   include <android/log.h>

#   ifndef UTILS_LOG_TAG
#       define UTILS_LOG_TAG "omre"
#   endif

#else

#   ifndef UTILS_LOG_TAG
#       define UTILS_LOG_TAG "omre"
#   endif

#endif

namespace utils {

namespace io {

ostream &LogStream::flush() noexcept {
    Buffer &buf = getBuffer();
    std::string tag = mTag.empty() ? UTILS_LOG_TAG : mTag;
#if ANDROID
    switch (mPriority) {
        case LOG_DEBUG:
            __android_log_write(ANDROID_LOG_DEBUG, tag.c_str(), buf.get());
            break;
        case LOG_ERROR:
            __android_log_write(ANDROID_LOG_ERROR, tag.c_str(), buf.get());
            break;
        case LOG_WARNING:
            __android_log_write(ANDROID_LOG_WARN, tag.c_str(), buf.get());
            break;
        case LOG_INFO:
            __android_log_write(ANDROID_LOG_INFO, tag.c_str(), buf.get());
            break;
    }
#else
    switch (mPriority) {
                case LOG_DEBUG:
                case LOG_WARNING:
                case LOG_INFO:
                    fprintf(stdout, "%s:%s", tag.c_str(), buf.get());
                    break;
                case LOG_ERROR:
                    fprintf(stderr, "%s:%s", tag.c_str(), buf.get());
                    break;
            }
#endif
    buf.reset();
    mTag = "";
    return *this;
}

static LogStream cout(LogStream::Priority::LOG_DEBUG);
static LogStream cerr(LogStream::Priority::LOG_ERROR);
static LogStream cwarn(LogStream::Priority::LOG_WARNING);
static LogStream cinfo(LogStream::Priority::LOG_INFO);

} // namespace io


const Loggers slog = {
        io::cout,   // debug
        io::cerr,   // error
        io::cwarn,  // warning
        io::cinfo   // info
};

} // namespace utils