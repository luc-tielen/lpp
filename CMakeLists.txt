cmake_minimum_required(VERSION 2.8)
project(lua++)

# Warnings + compiler specific flags:
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CXX_WARNINGS "-Werror -Weverything -pedantic -Wno-c++98-compat -Wno-exit-time-destructors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CXX_WARNINGS "-Werror -Wall -Wextra -pedantic -Wno-c++98-compat")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

# Compiler flags:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -march=native -fcolor-diagnostics")
#target_link_libraries(c++ c++abi)

# General project configuration:

subdirs(src tests)

