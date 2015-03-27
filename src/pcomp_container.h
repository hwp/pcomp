/* PCOMP
 * Copyright (c) 2015, Weipeng He <heweipeng@gmail.com>
 *
 * pcomp_container.h : container data structures
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

#ifndef PCOMP_CONTAINER_H_
#define PCOMP_CONTAINER_H_

#include <stdlib.h>

/**
 * primitive data types
 */
typedef enum {
  DATATYPE_INT,
  DATATYPE_UINT,
  DATATYPE_FLOAT,
  DATATYPE_DOUBLE,
  DATATYPE_PTR
} datatype_t;

size_t datatype_size(datatype_t type);

#define VOID_TO_INT(x) (*(int*) (x))
#define VOID_TO_UINT(x) (*(unsigned int*) (x))
#define VOID_TO_FLOAT(x) (*(float*) (x))
#define VOID_TO_DOUBLE(x) (*(double*) (x))
#define VOID_TO_PTR(x, type) (*(type**) (x))

/**
 * interface of a list
 */
typedef struct {
  /**
   * allocate a list object
   */
  void* (*alloc)(datatype_t type);

  /**
   * free a list object
   */
  void (*free)(void* obj);

  /**
   * element data type
   */
  datatype_t (*type)(void* obj);

  /**
   * number of elements
   */
  size_t (*size)(void* obj);

  /**
   * add an element to the end 
   */
  void (*add)(void* obj, void* elem);

  /**
   * get an element by index
   */
  void* (*get)(void* obj, size_t index);
} list_it;

/**
 * dynamic array
 */
typedef struct {
  datatype_t type;
  size_t size;
  size_t cap;

  void* data;
} darray_t;

darray_t* darray_alloc(datatype_t type);
void darray_free(darray_t* obj);
datatype_t darray_type(darray_t* obj);
size_t darray_size(darray_t* obj);
void darray_add(darray_t* obj, void* elem);
void* darray_get(darray_t* obj, size_t index);

/**
 * initialize a dynamic array interface.
 * the returned pointer should be freed using free().
 */
list_it* darray_init();

#endif  // PCOMP_CONTAINER_H_

