// common.c
// common functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "common.h"

#include <string.h>
#include <wchar.h>
#include <wctype.h>
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
  t = VOID_TO_PTR(darray_get(sym->symbols, id), wchar_t); 
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

static void lookup_add(symset_t* sym, wchar_t* t, darray_t* seq) {
  sid_t id = symset_lookup(sym, t);
  if (id == SID_NOMATCH) {
    id = symset_add(sym, t);
  }
  darray_push_back(seq, &id);
}

unsigned int char_encode(FILE* file, symset_t* sym, sid_t** text) {
  darray_t* seq = darray_alloc(sizeof(sid_t), NULL, NULL);

  wchar_t c;
  wchar_t t[] = L" ";

  while ((c = fgetwc(file)) != WEOF) {
    t[0] = c;
    lookup_add(sym, t, seq);
  }

  if (errno == EILSEQ) {
    perror("fgetwc");
    exit(EXIT_FAILURE);
  }

  unsigned int n = seq->size;
  *text = malloc(n * sizeof(sid_t));
  memcpy(*text, seq->data, n * sizeof(sid_t));

  darray_free(seq);
  return n;
}

#define WORD_ENCODE_BUFFER_SIZE 100

unsigned int word_encode(FILE* file, symset_t* sym, sid_t** text) {
  size_t buf_size = WORD_ENCODE_BUFFER_SIZE;
  wchar_t* buf = malloc(sizeof(wchar_t) * (buf_size + 1));
  darray_t* seq = darray_alloc(sizeof(sid_t), NULL, NULL);
  wchar_t c;
  unsigned int len = 0;

  while ((c = fgetwc(file)) != WEOF) {
    if (!iswalnum(c)) {
      if (len > 0) {
        buf[len] = L'\0';
        lookup_add(sym, buf, seq);
      }
      buf[0] = c;
      buf[1] = L'\0';
      lookup_add(sym, buf, seq);
      len = 0;
    }
    else {
      if (len >= buf_size) {
        buf_size *= 2;
        buf = realloc(buf, sizeof(wchar_t) * (buf_size + 1));
      }
      buf[len] = c;
      len++;
    }
  }

  if (errno == EILSEQ) {
    perror("fgetwc");
    exit(EXIT_FAILURE);
  }

  if (len > 0) {
    buf[len + 1] = L'\0';
    lookup_add(sym, buf, seq);
  }

  unsigned int n = seq->size;
  *text = malloc(n * sizeof(sid_t));
  memcpy(*text, seq->data, n * sizeof(sid_t));

  darray_free(seq);
  free(buf);
  return n;
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

