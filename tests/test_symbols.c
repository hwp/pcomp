// test_symbols.c
// test symbol encoding and decoding
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "cases.h"
#include "../src/symbol.h"

#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
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

HT_TEST(test_symset_add_lookup, void*) {
  sid_t id;
  symset_t* sym = symset_alloc();
  HT_ASSERT(sym->nsym == 0);

  id  = symset_lookup(sym, L"x");
  HT_ASSERT(id == SID_NOMATCH);
  sid_t xid = symset_add(sym, L"x");
  HT_ASSERT(xid != SID_NOMATCH);
  HT_ASSERT(sym->nsym == 1);
  id = symset_lookup(sym, L"x");
  HT_ASSERT(id == xid);
  id = symset_lookup(sym, L"z");
  HT_ASSERT(id == SID_NOMATCH);

  id  = symset_lookup(sym, L"y");
  HT_ASSERT(id == SID_NOMATCH);
  sid_t yid = symset_add(sym, L"y");
  HT_ASSERT(yid != SID_NOMATCH);
  HT_ASSERT(sym->nsym == 2);
  id = symset_lookup(sym, L"y");
  HT_ASSERT(id == yid);
  id = symset_lookup(sym, L"z");
  HT_ASSERT(id == SID_NOMATCH);

  wchar_t t[] = L"计";
  id  = symset_lookup(sym, t);
  HT_ASSERT(id == SID_NOMATCH);
  symset_add(sym, t);
  HT_ASSERT(sym->nsym == 3);
  id  = symset_lookup(sym, t);
  HT_ASSERT(id != SID_NOMATCH);

  t[0] = L'军';
  id  = symset_lookup(sym, t);
  HT_ASSERT(id == SID_NOMATCH);
  symset_add(sym, t);
  HT_ASSERT(sym->nsym == 4);
  id  = symset_lookup(sym, t);
  HT_ASSERT(id != SID_NOMATCH);

  symset_free(sym);
}

HT_TEST(test_char_encode_0, void*) {
  FILE* in = get_text(0);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = char_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 10);
  HT_ASSERT(size == 13);
  HT_ASSERT(text[0] != text[1]);
  HT_ASSERT(text[5] != text[11]);
  HT_ASSERT(text[2] == text[3]);
  HT_ASSERT(text[4] == text[7]);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_char_encode_1, void*) {
  FILE* in = get_text(1);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = char_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 42);
  HT_ASSERT(size == 5 * 80 - 8);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_char_encode_2, void*) {
  FILE* in = get_text(2);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = char_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 31);
  HT_ASSERT(size == 37);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_word_encode_0, void*) {
  FILE* in = get_text(0);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = word_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 5);
  HT_ASSERT(size == 5);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_word_encode_1, void*) {
  FILE* in = get_text(1);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = word_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 46);
  HT_ASSERT(size == 113);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_word_encode_2, void*) {
  FILE* in = get_text(2);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = word_encode(in, sym, &text);
  HT_ASSERT(sym->nsym == 8);
  HT_ASSERT(size == 9);

  symset_free(sym);
  free(text);
  fclose(in);
}

HT_TEST(test_cases, void*) {
  int i; 
  for (i = 0; i < case_max_id; i++) {
    FILE* in = get_text(i);
    HT_ASSERT(in != NULL);

    wprintf(L"Text %d:\n", i);
    wchar_t c;
    while ((c = fgetwc(in)) != EOF) {
      fputwc(c, stdout);
    }
    wprintf(L"\n");

    if (errno == EILSEQ) {
      perror("fgetwc");
      exit(EXIT_FAILURE);
    }
    
    fclose(in);
  }
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");

  ht_suit_t* suit = ht_suit_alloc(NULL); 

  ht_option_t option;
  ht_get_option(argc, argv, &option);

  ht_add_test(suit, test_cases);
  ht_add_test(suit, test_symset_alloc);
  ht_add_test(suit, test_symset_add_lookup);
  ht_add_test(suit, test_char_encode_0);
  ht_add_test(suit, test_char_encode_1);
  ht_add_test(suit, test_char_encode_2);
  ht_add_test(suit, test_word_encode_0);
  ht_add_test(suit, test_word_encode_1);
  ht_add_test(suit, test_word_encode_2);

  int ret = ht_run_suit(suit, &option);
  ht_suit_free(suit);

  if (ret == 0) {
    exit(EXIT_SUCCESS);
  }
  else {
    exit(EXIT_FAILURE);
  }
}

