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

#ifndef OMRE_UTILS_RWLOCK_H
#define OMRE_UTILS_RWLOCK_H

#include <cstddef>

#include <utils/Condition.h>
#include <utils/Mutex.h>

namespace utils {

#if !defined(_WIN32)

/**
 * Simple mutex class.  The implementation is system-dependent.
 *
 * The mutex must be unlocked by the thread that locked it. They are not
 * recursive, i.e. the same thread can't lock it multiple times.
 */
class RWLock {
public:
    RWLock() : nWaitReaders(0), nWaitWriters(0), refcount(0) {}
    RWLock(const RWLock &) = delete;
    RWLock &operator=(const RWLock&) = delete;
    ~RWLock() = default;

    void readLock();
    bool tryReadLock();
    void writeLock();
    bool tryWriteLock();
    void unlock();

private:
    mutable Mutex lock_;
    mutable Condition notify_readers_;
    mutable Condition notify_writers_;
    //!< the number of waiting readers
    int nWaitReaders;
    //!< the number of waiting writers
    int nWaitWriters;
    //!< 0: not locked; -1: locked by one writer; > 0: locked by refcount readers
    int refcount;
};

#endif // !defined(_WIN32)

} // namespace utils

#endif //OMRE_UTILS_RWLOCK_H
