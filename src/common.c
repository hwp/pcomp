// common.c
// common functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "common.h"

#include <string.h>
#include <wchar.h>
#include <assert.h>
#include <errno.h>
#include <hwp_utils/list.h>
#include <hwp_utils/map.h>

symset_t* symset_alloc() {
  symset_t* ret = malloc(sizeof(symset_t));
  assert(ret);
  ret->nsym = 0;
  ret->symbols = darray_alloc(sizeof(wchar_t*), (dup_f) wcsdup, free);
  ret->indices = hashmap_alloc(sizeof(wchar_t*), NULL,
      NULL, sizeof(sid_t), NULL, NULL, HASHMAP_DEFAULT_NBINS,
      hash_wcstr, NULL, compar_wcstr, NULL);

  return ret;
}

void symset_free(symset_t* sym) {
  if (sym) {
    darray_free(sym->symbols);
    hashmap_free(sym->indices);
    free(sym);
  }
}

sid_t symset_add(symset_t* sym, wchar_t* t) {
  sid_t id = sym->nsym;

  darray_push_back(sym->symbols, &t);
  hashmap_put(sym->indices, &t, &id);

  sym->nsym++;
  assert(sym->symbols->size == sym->nsym);
  assert(sym->indices->size == sym->nsym);
  return id;
}

sid_t symset_lookup(symset_t* sym, wchar_t* t) {
  void* v = hashmap_get(sym->indices, &t);
  if (v) {
    return VOID_TO_TYPE(v, sid_t);
  }
  else {
    return SID_NOMATCH;
  }
}

unsigned int char_encode(FILE* file, symset_t* sym, sid_t** text) {
  darray_t* seq = darray_alloc(sizeof(sid_t), NULL, NULL);

  wchar_t c;
  wchar_t t[] = L" ";

  while ((c = fgetwc(file)) != WEOF) {
    t[0] = c;
    sid_t id = symset_lookup(sym, t);
    if (id == SID_NOMATCH) {
      id = symset_add(sym, t);
    }
    darray_push_back(seq, &id);
  }

  if (errno == EILSEQ) {
    perror("fgetwc");
    exit(EXIT_FAILURE);
  }

  unsigned int n = seq->size;
  *text = malloc(seq->size * sizeof(sid_t));
  memcpy(*text, seq->data, seq->size * sizeof(sid_t));

  darray_free(seq);
  return n;
}

unsigned int word_encode(FILE* file, symset_t* sym, sid_t** text) {
  return 0;
}

unsigned int symbol_decode(FILE* file, symset_t* sym, sid_t* text, unsigned int size) {
  unsigned int i;

  for (i = 0; i < size; i++) {
    sid_t s = text[i];
    assert(s < sym->nsym);
    fwprintf(file, L"%ls", VOID_TO_PTR(darray_get(sym->symbols, s), wchar_t));
  }

  return size;
}

