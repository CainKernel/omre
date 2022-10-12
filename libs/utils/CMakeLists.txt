cmake_minimum_required(VERSION 3.4.1)
project(utils)

set(TARGET utils)
set(TARGET_LINUX ${TARGET}/linux)
set(TARGET_GENERIC ${TARGET}/generic)
set(PUBLIC_HDR_DIR include)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
file(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)

set(DIST_HDRS
    ${PUBLIC_HDR_DIR}/${TARGET}/algorithm.h
    ${PUBLIC_HDR_DIR}/${TARGET}/arraysize.h
    ${PUBLIC_HDR_DIR}/${TARGET}/bitset.h
    ${PUBLIC_HDR_DIR}/${TARGET}/CallStack.h
    ${PUBLIC_HDR_DIR}/${TARGET}/compiler.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Condition.h
    ${PUBLIC_HDR_DIR}/${TARGET}/CountDownLatch.h
    ${PUBLIC_HDR_DIR}/${TARGET}/CString.h
    ${PUBLIC_HDR_DIR}/${TARGET}/CyclicBarrier.h
    ${PUBLIC_HDR_DIR}/${TARGET}/debug.h
    ${PUBLIC_HDR_DIR}/${TARGET}/export.h
    ${PUBLIC_HDR_DIR}/${TARGET}/GuidUtils.h
    ${PUBLIC_HDR_DIR}/${TARGET}/inline.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Log.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Mutex.h
    ${PUBLIC_HDR_DIR}/${TARGET}/ostream.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Panic.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Path.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Range.h
    ${PUBLIC_HDR_DIR}/${TARGET}/SpinLock.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Systrace.h
    ${PUBLIC_HDR_DIR}/${TARGET}/Thread.h
    ${PUBLIC_HDR_DIR}/${TARGET}/ThreadUtils.h
    ${PUBLIC_HDR_DIR}/${TARGET}/time.h
)

set(DIST_LINUX_HDRS
    ${PUBLIC_HDR_DIR}/${TARGET_LINUX}/Condition.h
    ${PUBLIC_HDR_DIR}/${TARGET_LINUX}/Mutex.h
)

set(DIST_GENERIC_HDRS
    ${PUBLIC_HDR_DIR}/${TARGET_GENERIC}/Condition.h
    ${PUBLIC_HDR_DIR}/${TARGET_GENERIC}/Mutex.h
)

set(SRCS
    src/CallStack.cpp
    src/CountDownLatch.cpp
    src/CString.cpp
    src/CyclicBarrier.cpp
    src/debug.cpp
    src/GuidUtils.cpp
    src/Log.cpp
    src/ostream.cpp
    src/Panic.cpp
    src/Path.cpp
    src/RWLock.cpp
    src/Systrace.cpp
    src/Thread.cpp
    src/ThreadUtils.cpp
    src/time.cpp
)

if (WIN32)
    list(APPEND SRCS src/win32/Path.cpp)
endif()
if (LINUX OR ANDROID)
    list(APPEND SRCS src/linux/Condition.cpp)
    list(APPEND SRCS src/linux/Mutex.cpp)
    list(APPEND SRCS src/linux/Path.cpp)
endif()
if (APPLE)
    list(APPEND SRCS src/darwin/Path.mm)
endif()

if (ANDROID)
    list(APPEND DIST_HDRS
        ${PUBLIC_HDR_DIR}/${TARGET}/api_level.h
        ${PUBLIC_HDR_DIR}/${TARGET}/JNIHelp.h
        ${PUBLIC_HDR_DIR}/${TARGET}/VirtualMachineEnv.h
    )
    list(APPEND SRCS
        src/android/JNIHelp.cpp
        src/android/VirtualMachineEnv.cpp
        src/api_level.cpp
    )
endif()

# ==================================================================================================
# Include and target definitions
# ==================================================================================================
include_directories(${PUBLIC_HDR_DIR})

add_library(${TARGET} STATIC ${PUBLIC_HDRS} ${SRCS})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

if (ANDROID)
    target_link_libraries(${TARGET} PRIVATE log)
    target_link_libraries(${TARGET} PRIVATE dl)
    target_link_libraries(${TARGET} PUBLIC android)
endif()

if (WIN32)
    # Needed for shlwapi.h (GetModuleFileName)
    target_link_libraries(${TARGET} PUBLIC Shlwapi)
endif()

if (LINUX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET} PRIVATE Threads::Threads)
    target_link_libraries(${TARGET} PRIVATE dl)
endif()

# ==================================================================================================
# Installation
# ==================================================================================================
set(INSTALL_TYPE ARCHIVE)
install(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${DIST_DIR})
install(FILES ${DIST_HDRS} DESTINATION include/${TARGET})
if (LINUX OR ANDROID)
    install(FILES ${DIST_LINUX_HDRS} DESTINATION include/${TARGET_LINUX})
else()
    install(FILES ${DIST_GENERIC_HDRS} DESTINATION include/${TARGET_GENERIC})
endif()
