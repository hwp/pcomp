# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

add_executable(check_list check_list.c)
target_link_libraries(check_list pcomp_lib ${CHECK_LIBRARIES})

add_test(NAME check_list COMMAND check_list)

