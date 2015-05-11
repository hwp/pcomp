# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(pcomp)

include(FindPkgConfig)

pkg_check_modules(UTILS REQUIRED hwp_utils)
include_directories(${UTILS_INCLUDE_DIRS})

if(UNIX)
  set(MATH_LIBRARY m)
endif(UNIX)

set(CMAKE_C_FLAGS "-Wall")
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

