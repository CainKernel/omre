cmake_minimum_required(VERSION 3.4.1)
project(sonic)

set(TARGET sonic)
set(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
set(PUBLIC_HDRS
    ${OUR_DIR}/sonic.h
)

set(SRCS
    ${OUR_DIR}/sonic.c
)

# ==================================================================================================
# Include and target definitions
# ==================================================================================================
include_directories(${OUR_DIR})

add_library(${TARGET} STATIC ${PUBLIC_HDRS} ${SRCS})
target_compile_options(${TARGET} PRIVATE ${OPTIMIZATION_FLAGS})
target_include_directories(${TARGET} PUBLIC ${OUR_DIR})

# ==================================================================================================
# Installation
# ==================================================================================================
install(DIRECTORY ${OUR_DIR}/sonic DESTINATION include)