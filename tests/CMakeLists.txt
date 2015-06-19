# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2015, All rights reserved.

add_executable(test_symbols test_symbols.c cases.c ../src/common.c)
target_link_libraries(test_symbols ${UTILS_LIBRARIES})
add_test(NAME test_symbols COMMAND test_symbols)

