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

#ifndef OMRE_UTILS_INLINE_H
#define OMRE_UTILS_INLINE_H

#if defined(_MSC_VER)

#define OMRE_FORCE_INLINE __forceinline
#define OMRE_NO_INLINE __declspec(noinline)

#elif defined(__GNUC__)

#define OMRE_FORCE_INLINE __attribute__((__always_inline__))
#define OMRE_NO_INLINE __attribute__((__noinline__))

#else

#define OMRE_FORCE_INLINE
#define OMRE_NO_INLINE

#endif

#endif //OMRE_UTILS_INLINE_H
