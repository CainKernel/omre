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

#ifndef OMRE_UTILS_EXPORT_H
#define OMRE_UTILS_EXPORT_H

// OMRE_EXPORT is used to mark symbols as exported or imported when omre is
// built or used as a shared library.
// When omre is built as a static library the OMRE_EXPORT macro expands to
// nothing.

#ifdef OMRE_ENABLE_SYMBOL_EXPORT

#ifdef OMRE_WIN

#ifdef OMRE_LIBRARY_IMPL
#define OMRE_EXPORT __declspec(dllexport)
#else
#define OMRE_EXPORT __declspec(dllimport)
#endif

#else  // OMRE_WIN

#if __has_attribute(visibility) && defined(OMRE_LIBRARY_IMPL)
#define OMRE_EXPORT __attribute__((visibility("default")))
#endif

#endif  // OMRE_WIN

#endif  // OMRE_ENABLE_SYMBOL_EXPORT

#ifndef OMRE_EXPORT
#define OMRE_EXPORT
#endif

#endif //OMRE_UTILS_EXPORT_H
