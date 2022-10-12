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

#ifndef OMRE_UTILS_GUID_UTILS_H
#define OMRE_UTILS_GUID_UTILS_H

#include <string>

namespace utils {

class GuidUtils {
public:
    static constexpr int kDefaultGuidLength = 32;

    /**
     * Creates a new guid
     */
    static std::string create(int len = kDefaultGuidLength);

private:
    GuidUtils() = default;
    ~GuidUtils() = default;
};

} // namespace utils

#endif //OMRE_UTILS_GUID_UTILS_H
