# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2015, All rights reserved.

file(COPY texts DESTINATION .)
set(TEXT_PATH ${CMAKE_CURRENT_BINARY_DIR}/texts)

configure_file(test_config.h.in config/test_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)

add_executable(test_symbols test_symbols.c cases.c ../src/common.c)
target_link_libraries(test_symbols ${UTILS_LIBRARIES})
add_test(NAME test_symbols COMMAND test_symbols)

