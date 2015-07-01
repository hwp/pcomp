// test_bitio.c
// test bit-wise input/output
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "../src/bitio.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <hwp_utils/test.h>

HT_TEST(test_eof, void*) {
  HT_ASSERT(BIT_EOF != 0);
  HT_ASSERT(BIT_EOF != 1);
}

HT_TEST(test_empty, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  HT_ASSERT(io != NULL);
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  HT_ASSERT(io != NULL);
  bit b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

HT_TEST(test_one, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  bitio_fputb(1, io);
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  bit b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

HT_TEST(test_10110, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  bitio_fputb(1, io);
  bitio_fputb(0, io);
  bitio_fputb(1, io);
  bitio_fputb(1, io);
  bitio_fputb(0, io);
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  bit b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 0);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 0);
  b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

HT_TEST(test_10110111, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  bitio_fputb(1, io);
  bitio_fputb(0, io);
  bitio_fputb(1, io);
  bitio_fputb(1, io);
  bitio_fputb(0, io);
  bitio_fputb(1, io);
  bitio_fputb(1, io);
  bitio_fputb(1, io);
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  bit b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 0);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 0);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == 1);
  b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

HT_TEST(test_buffer_full, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  int i;
  for (i = 0; i < BITIO_BUFFER_SIZE * CHAR_BIT; i++) {
    bitio_fputb(i % 5 ? 0 : 1, io);
  }
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  bit b;
  for (i = 0; i < BITIO_BUFFER_SIZE * CHAR_BIT; i++) {
    b = bitio_fgetb(io);
    HT_ASSERT(b == (i % 5 ? 0 : 1));
  }
  b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

HT_TEST(test_buffer_half, void*) {
  FILE* file = tmpfile();

  bitio_t* io = bitio_attach(file);
  int i;
  for (i = 0; i < BITIO_BUFFER_SIZE * CHAR_BIT + 13; i++) {
    bitio_fputb(i % 5 ? 0 : 1, io);
  }
  bitio_detach(io);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);
  bit b;
  for (i = 0; i < BITIO_BUFFER_SIZE * CHAR_BIT + 13; i++) {
    b = bitio_fgetb(io);
    HT_ASSERT(b == (i % 5 ? 0 : 1));
  }
  b = bitio_fgetb(io);
  HT_ASSERT(b == BIT_EOF);
  bitio_detach(io);

  fclose(file);
}

int main(int argc, char** argv) {
  ht_suit_t* suit = ht_suit_alloc(NULL); 

  ht_option_t option;
  ht_get_option(argc, argv, &option);

  ht_add_test(suit, test_eof);
  ht_add_test(suit, test_empty);
  ht_add_test(suit, test_one);
  ht_add_test(suit, test_10110);
  ht_add_test(suit, test_10110111);
  ht_add_test(suit, test_buffer_full);
  ht_add_test(suit, test_buffer_half);

  int ret = ht_run_suit(suit, &option);
  ht_suit_free(suit);

  if (ret == 0) {
    exit(EXIT_SUCCESS);
  }
  else {
    exit(EXIT_FAILURE);
  }
}

