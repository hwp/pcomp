// cases.c
// text cases
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "cases.h"

#include <stdio.h>
#include <string.h>

static char t0[] = "Hello World!";

static char t1[] = "aaaaaaaaab";

static char t2[] = "iiiii";

static char t3[] = {
  'G', 0xc3, 0xb6, 'd', 'e', 'l', 's', 'c', 'h', 'e', 'r',
  ' ', 'V', 'o', 'l', 'l', 's', 't', 0xc3, 0xa4, 'n', 'd',
  'i', 'g', 'k', 'e', 'i', 't', 's', 's', 'a', 't', 'z'
};

static char t4[] = {
  0xe5, 0x91, 0xb5, 0xe5, 0x91, 0xb5, 0x3a, 0x44 
};

static char* texts[] = {t0, t1, t2, t3, t4};

int case_max_id = 5;

FILE* get_text(int id) {
  id %= case_max_id;
  FILE* ret = NULL;

  switch (id) {
    case 0:
    case 1:
    case 2:
      ret = fmemopen(texts[id], strlen(texts[id]), "r");
      break;
    case 3:
    case 4:
      ret = fmemopen(texts[id], sizeof(texts[id]), "r");
      break;
  }
  
  return ret;
}

