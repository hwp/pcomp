# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

add_executable(entropy entropy.c symbol.c)
target_link_libraries(entropy ${UTILS_LIBRARIES} ${MATH_LIBRARY})

