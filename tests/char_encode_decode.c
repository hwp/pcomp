// char_encode_decode.c
// char_encode and symbol_decode a stream
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "../src/common.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char** argv) {
  assert(argc == 2);
  setlocale(LC_ALL, "");

  FILE* in = fopen(argv[1], "r");
  assert(in);

  symset_t* sym = symset_alloc();
  sid_t* text = NULL;

  unsigned int size = char_encode(in, sym, &text);
  unsigned int dsize = symbol_decode(stdout, sym, text, size);

  assert(size == dsize);

  symset_free(sym);
  free(text);
  fclose(in);

  exit(EXIT_SUCCESS);
}

