/* PCOMP
 * Copyright (c) 2015, Weipeng He <heweipeng@gmail.com>
 *
 * check_list.c : test lists
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General
 * Public License along with this program; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "../src/pcomp_container.h"

#include <stdlib.h>
#include <check.h>

START_TEST(test_datatype_size) {
  ck_assert(datatype_size(DATATYPE_INT) == sizeof(int));
  ck_assert(datatype_size(DATATYPE_UINT) == sizeof(unsigned int));
  ck_assert(datatype_size(DATATYPE_FLOAT) == sizeof(float));
  ck_assert(datatype_size(DATATYPE_DOUBLE) == sizeof(double));
  ck_assert(datatype_size(DATATYPE_PTR) == sizeof(void*));
  ck_assert(sizeof(int*) == sizeof(void*));
}
END_TEST

START_TEST(test_darray_init) {
  list_it* darray = darray_init();

  ck_assert(darray);
  ck_assert(darray->alloc);
  ck_assert(darray->free);
  ck_assert(darray->type);
  ck_assert(darray->size);
  ck_assert(darray->add);
  ck_assert(darray->get);

  free(darray);
}
END_TEST

START_TEST(test_darray_int) {
  list_it* darray = darray_init();
  void* list = darray->alloc(DATATYPE_INT);
  ck_assert(darray->type(list) == DATATYPE_INT);
  ck_assert(darray->size(list) == 0);

  int i;
  for (i = 0; i < 2000; i++) {
    int x = 2 * i - i * i + 1000;
    darray->add(list, &x);
  };

  for (i = 0; i < 2000; i++) {
    ck_assert(VOID_TO_INT(darray->get(list, i)) == 2 * i - i * i + 1000);
  };

  darray->free(list);
  free(darray);
}
END_TEST

Suite* list_suite(void) {
  Suite* s;

  s = suite_create("list");

  // data type
  TCase* tc_type = tcase_create("datatype");
  tcase_add_test(tc_type, test_datatype_size);

  suite_add_tcase(s, tc_type);

  // darray
  TCase* tc_darray = tcase_create("darray");
  tcase_add_test(tc_darray, test_darray_init);
  tcase_add_test(tc_darray, test_darray_int);
  suite_add_tcase(s, tc_darray);

  return s;
}

int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = list_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

