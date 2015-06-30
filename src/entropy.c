// entropy.c
// calculate the entropy of a document
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <math.h>
#include <wchar.h>
#include <assert.h>

#include <hwp_utils/debug.h>

int main(int argc, char** argv) {
  debug_init();
  setlocale(LC_ALL, "");

  int showhelp = 0;
  int list = 0;
  int type = 0;
  int opt;
  while ((opt = getopt(argc, argv, "lt:h")) != -1) {
    switch (opt) {
      case 'h':
        showhelp = 1;
        break;
      case 'l':
        list = 1;
        break;
      case 't':
        type = atoi(optarg);
        break;
      default:
        showhelp = 1;
        break;
    }
  }

  if (showhelp || type < 0 || type > 1) {
    fprintf(stderr, "Usage: %s -t type [-l] [file]\n", argv[0]);
    fprintf(stderr, "\tuse stdin if file not given\n");
    fprintf(stderr, "\ttype 0: character-wise\n");
    fprintf(stderr, "\ttype 1: word-wise\n");
    fprintf(stderr, "\t-l: list all symbols\n");
    exit(EXIT_SUCCESS);
  }

  FILE* file = stdin;
  if (argc - optind == 1) {
    file = fopen(argv[optind], "r");
  }

  symset_t* set = symset_alloc();
  sid_t* seq = NULL;
  unsigned int n = 0;
  switch (type) {
    case 0:
      n = char_encode(file, set, &seq);
      break;
    case 1:
      n = word_encode(file, set, &seq);
      break;
    default:
      assert(0);
      break;
  }

  unsigned int* counter = calloc(set->nsym,
      sizeof(unsigned int));
  unsigned int i;
  for (i = 0; i < n; i++) {
    counter[seq[i]]++;
  }

  if (list) {
    for (i = 0; i < set->nsym; i++) {
      wchar_t* str = escape_wcstr(VOID_TO_PTR(
            darray_get(set->symbols, i), wchar_t));
      wprintf(L"%ls: %zd\n", str, counter[i]);
      free(str);
    }
    wprintf(L"total: %zd\n", n);
  }

  double total = (double) n;

  size_t s = 0;
  size_t ts = 0;
  for (i = 0; i < set->nsym; i++) {
    size_t t = wcstombs(NULL, VOID_TO_PTR(
          darray_get(set->symbols, i), wchar_t), 0);
    s += t;
    ts += t * counter[i];
  }
  wprintf(L"size of original file: %zd bytes\n", ts);
  wprintf(L"# of symbols: %zd\n", set->nsym);
  wprintf(L"length of text: %zd\n", n);
  wprintf(L"size of symbols (without compression): %zd bytes\n", s);

  double h0 = log((double) set->nsym) / log(2.0);
  wprintf(L"zeroth order entropy: %g bits\n", h0);
  wprintf(L"zeroth order encoding: %g (%g) bytes\n", h0 * total / 8.0, h0 * total / 8.0 + s);

  double h1 = 0.0;
  for (i = 0; i < set->nsym; i++) {
    double p = (double) counter[i] / total;
    h1 -= p * (log(p) / log(2.0));
  }
  wprintf(L"first order entropy: %g bits\n", h1);
  wprintf(L"first order encoding: %g (%g) bytes\n", h1 * total / 8.0, h1 * total / 8.0 + s);

  if (argc - optind == 1) {
    fclose(file);
  }

  symset_free(set);
  free(seq);
  free(counter);
  exit(EXIT_SUCCESS);
}

