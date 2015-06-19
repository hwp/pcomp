// common.c
// common functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "common.h"

symset_t* symset_alloc() {
  return NULL;
}

void symset_free(symset_t* sym) {

}

unsigned int char_encode(FILE* file, symset_t* sym, sid_t** text) {
  return 0;
}

unsigned int word_encode(FILE* file, symset_t* sym, sid_t** text) {
  return 0;
}

unsigned int symbol_decode(FILE* file, symset_t* sym, sid_t* text, unsigned int size) {
  return 0;
}

