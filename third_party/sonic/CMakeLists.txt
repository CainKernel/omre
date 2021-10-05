cmake_minimum_required(VERSION 3.4.1)
project(sonic)

set(TARGET sonic)
set(PUBLIC_HDR_DIR include)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
set(PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/sonic.h
)

set(SRCS
        src/sonic.c
)

# ==================================================================================================
# Include and target definitions
# ==================================================================================================
include_directories(${PUBLIC_HDR_DIR})

add_library(${TARGET} STATIC ${PUBLIC_HDRS} ${SRCS})
target_compile_options(${TARGET} PRIVATE ${OPTIMIZATION_FLAGS})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

# ==================================================================================================
# Installation
# ==================================================================================================
install(DIRECTORY ${PUBLIC_HDR_DIR}/sonic DESTINATION include)