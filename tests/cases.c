// cases.c
// text cases
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "cases.h"
#include "test_config.h"

#include <stdio.h>
#include <unistd.h>

int case_max_id = 3;

FILE* get_text(int id) {
  chdir(TEXT_PATH);

  id %= case_max_id;
  char path[] = "text0";
  path[4] = '0' + id;

  FILE* ret = fopen(path, "r");
  if (ret == NULL) {
    perror("fopen");
  }

  return ret;
}

