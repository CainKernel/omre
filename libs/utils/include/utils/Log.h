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

#ifndef OMRE_UTILS_LOG_H
#define OMRE_UTILS_LOG_H

#include <string>

#include <utils/compiler.h>

#include <utils/ostream.h>

namespace utils {

namespace io {

class UTILS_PUBLIC LogStream : public ostream {
public:

    enum Priority {
        LOG_DEBUG, LOG_ERROR, LOG_WARNING, LOG_INFO
    };

    explicit LogStream(Priority p) noexcept: mPriority(p) {}

    ostream &flush() noexcept override;

    LogStream &tag(std::string tag) noexcept {
        mTag = std::move(tag);
        return *this;
    }

private:
    Priority mPriority;
    std::string mTag;
};

} // namespace io

struct UTILS_PUBLIC Loggers {

    // DEBUG level logging stream
    io::LogStream &d;

    // ERROR level logging stream
    io::LogStream &e;

    // WARNING level logging stream
    io::LogStream &w;

    // INFORMATION level logging stream
    io::LogStream &i;
};

extern UTILS_PUBLIC const Loggers slog;

} // namespace utils

#endif //OMRE_UTILS_LOG_H
