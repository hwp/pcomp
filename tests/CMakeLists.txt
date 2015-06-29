# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2015, All rights reserved.

file(COPY texts DESTINATION .)
set(TEXT_PATH ${CMAKE_CURRENT_BINARY_DIR}/texts)

configure_file(test_config.h.in config/test_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)

configure_file(run_and_diff.sh run_and_diff.sh COPYONLY)

add_executable(test_symbols
  test_symbols.c cases.c ../src/common.c)
target_link_libraries(test_symbols ${UTILS_LIBRARIES})
add_test(NAME test_symbols COMMAND test_symbols)

add_executable(char_encode_decode 
  char_encode_decode.c ../src/common.c)
target_link_libraries(char_encode_decode ${UTILS_LIBRARIES})
add_test(NAME char_encode_decode_0 
  COMMAND run_and_diff.sh ./char_encode_decode ${TEXT_PATH}/text0 ${TEXT_PATH}/text0)
add_test(NAME char_encode_decode_1 
  COMMAND run_and_diff.sh ./char_encode_decode ${TEXT_PATH}/text1 ${TEXT_PATH}/text1)
add_test(NAME char_encode_decode_2 
  COMMAND run_and_diff.sh ./char_encode_decode ${TEXT_PATH}/text2 ${TEXT_PATH}/text2)

add_executable(word_encode_decode 
  word_encode_decode.c ../src/common.c)
target_link_libraries(word_encode_decode ${UTILS_LIBRARIES})
add_test(NAME word_encode_decode_0 
  COMMAND run_and_diff.sh ./word_encode_decode ${TEXT_PATH}/text0 ${TEXT_PATH}/text0)
add_test(NAME word_encode_decode_1 
  COMMAND run_and_diff.sh ./word_encode_decode ${TEXT_PATH}/text1 ${TEXT_PATH}/text1)
add_test(NAME word_encode_decode_2 
  COMMAND run_and_diff.sh ./word_encode_decode ${TEXT_PATH}/text2 ${TEXT_PATH}/text2)

