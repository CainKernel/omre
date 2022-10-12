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

#ifndef OMRE_ANDROID_LOG_H
#define OMRE_ANDROID_LOG_H

#define JNI_TAG "Omre"

#if defined(__ANDROID__)

#include <android/log.h>

#define ALOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG, format, ##__VA_ARGS__)
#define ALOGI(format, ...) __android_log_print(ANDROID_LOG_INFO,  JNI_TAG, format, ##__VA_ARGS__)
#define ALOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG, format, ##__VA_ARGS__)
#define ALOGW(format, ...) __android_log_print(ANDROID_LOG_WARN,  JNI_TAG, format, ##__VA_ARGS__)
#define ALOGV(format, ...) __android_log_print(ANDROID_LOG_VERBOSE,  JNI_TAG, format, ##__VA_ARGS__)

#else

#define ALOGE(format, ...) {}
#define ALOGI(format, ...) {}
#define ALOGD(format, ...) {}
#define ALOGW(format, ...) {}
#define ALOGV(format, ...) {}

#endif

#endif //OMRE_ANDROID_LOG_H
