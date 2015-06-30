# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

add_executable(entropy entropy.c common.c)
target_link_libraries(entropy ${UTILS_LIBRARIES} ${MATH_LIBRARY})

