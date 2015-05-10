// entropy.c
// calculate the entropy of a document
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include <hwp_utils/map.h>
#include <hwp_utils/debug.h>

#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <errno.h>


int main(int argc, char** argv) {
  debug_init();
  setlocale(LC_ALL, "");

  int showhelp = 0;
  int type = 0;
  int opt;
  while ((opt = getopt(argc, argv, "t:h")) != -1) {
    switch (opt) {
      case 'h':
        showhelp = 1;
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
    fprintf(stderr, "Usage: %s -t type file\n", argv[0]);
    fprintf(stderr, "\ttype 0: character-wise\n");
    exit(EXIT_SUCCESS);
  }

  FILE* file = stdin;
  if (argc - optind == 1) {
    file = fopen(argv[optind], "r");
  }
  wchar_t c;
  size_t n = 0;
  hashmap_t* counter = hashmap_alloc(sizeof(wchar_t),
      NULL, NULL, sizeof(unsigned int), NULL, NULL,
      HASHMAP_DEFAULT_NBINS, hash_wchar, NULL,
      compar_wchar, NULL);
  while ((c = fgetwc(file)) != WEOF) {
    void* v = hashmap_get(counter, &c);
    unsigned int x = 0;
    if (v) {
      x = VOID_TO_UINT(v);
    }
    x++;
    hashmap_put(counter, &c, &x);
    n++;
  }

  if (errno == EILSEQ) {
    perror("fgetwc");
    exit(EXIT_FAILURE);
  }

  HASHMAP_FOR(counter, wchar_t, k, unsigned int, v) {
    switch (k) {
      case L'\n':
        wprintf(L"\\n:%d\n", v);
        break;
      case L'\r':
        wprintf(L"\\r:%d\n", v);
        break;
      case L'\t':
        wprintf(L"\\t:%d\n", v);
        break;
      default:
        wprintf(L"%lc:%d\n", k, v);
        break;
    }
  } HASHMAP_ENDFOR

  wprintf(L"%zd\n", n);
  if (argc - optind == 1) {
    fclose(file);
  }

  hashmap_free(counter);
  exit(EXIT_SUCCESS);
}

