// test_symbols.c
// test symbol encoding and decoding
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "cases.h"
#include "../src/common.h"

#include <stdlib.h>
#include <wchar.h>
#include <hwp_utils/test.h>

HT_TEST(test_symset_alloc, void*) {
  symset_t* sym = symset_alloc();
  HT_ASSERT(sym != NULL);
  HT_ASSERT(sym->nsym == 0);
  HT_ASSERT(sym->symbols->size == 0);
  HT_ASSERT(sym->symbols->elem_size == sizeof(wchar_t*));
  HT_ASSERT(sym->indices->size == 0);
  HT_ASSERT(sym->indices->key_size == sizeof(wchar_t*));
  HT_ASSERT(sym->indices->value_size == sizeof(sid_t));
  symset_free(sym);
}

HT_TEST(test_char_encode_0, void*) {
  FILE* in = get_text(0);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = char_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 9);
  HT_ASSERT(size == 12);
  HT_ASSERT(text[0] != text[1]);
  HT_ASSERT(text[5] != text[11]);
  HT_ASSERT(text[2] == text[3]);
  HT_ASSERT(text[4] == text[7]);

  symset_free(sym);
  fclose(in);
}

int main(int argc, char** argv) {
  ht_suit_t* suit = ht_suit_alloc(NULL); 

  ht_option_t option;
  ht_get_option(argc, argv, &option);

  ht_add_test(suit, test_symset_alloc);
  ht_add_test(suit, test_char_encode_0);

  int ret = ht_run_suit(suit, &option);
  ht_suit_free(suit);

  if (ret == 0) {
    exit(EXIT_SUCCESS);
  }
  else {
    exit(EXIT_FAILURE);
  }
}

