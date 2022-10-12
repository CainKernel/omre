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

#include <utils/RWLock.h>

namespace utils {

void RWLock::readLock() {
    std::unique_lock<Mutex> guard(lock_);
    {
        while (refcount < 0 || nWaitWriters > 0) {
            nWaitReaders++;
            notify_readers_.wait(guard);
            nWaitReaders--;
        }
        refcount++;
    }
    guard.unlock();
}

bool RWLock::tryReadLock() {
    bool res = true;
    std::unique_lock<Mutex> guard(lock_);
    {
        if (refcount < 0 || nWaitWriters > 0) {
            res = false;
        } else {
            //!< increment count of reader locks
            refcount++;
        }
    }
    guard.unlock();
    return res;
}

void RWLock::writeLock() {
    std::unique_lock<Mutex> guard(lock_);
    {
        while (refcount != 0) {
            nWaitWriters++;
            notify_writers_.wait(guard);
            nWaitWriters--;
        }
        //!< acquire the write lock
        refcount = -1;
    }
    guard.unlock();
}

bool RWLock::tryWriteLock() {
    bool res = true;
    std::unique_lock<Mutex> guard(lock_);
    {
        if (refcount != 0) {
            res = false;
        } else {
            // acquire the write lock
            refcount = -1;
        }
    }
    guard.unlock();
    return res;
}

void RWLock::unlock() {
    std::unique_lock<Mutex> guard(lock_);
    {
        if (refcount > 0) {
            refcount--;
        } else if (refcount == -1) {
            refcount = 0;
        }
        if (nWaitWriters > 0) {
            if (refcount == 0) {
                notify_writers_.notify_one();
            }
        } else if (nWaitReaders > 0) {
            notify_readers_.notify_all();
        }
    }
    guard.unlock();
}

} // namespace utils
