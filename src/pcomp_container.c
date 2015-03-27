/* PCOMP
 * Copyright (c) 2015, Weipeng He <heweipeng@gmail.com>
 *
 * pcomp_container.c : container data structures
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

#include "pcomp_container.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DARRAY_INIT_CAP 128

size_t datatype_size(datatype_t type) {
  size_t size;
  switch (type) {
    case DATATYPE_INT:
      size = sizeof(int);
      break;
    case DATATYPE_UINT:
      size = sizeof(unsigned int);
      break;
    case DATATYPE_FLOAT:
      size = sizeof(float);
      break;
    case DATATYPE_DOUBLE:
      size = sizeof(double);
      break;
    case DATATYPE_PTR:
      size = sizeof(void*);
      break;
    default:
      assert(0);
      break;
  }
  
  return size;
}

darray_t* darray_alloc(datatype_t type) {
  darray_t* obj = malloc(sizeof(darray_t));
  assert(obj);

  obj->type = type;
  obj->size = 0;
  obj->cap = DARRAY_INIT_CAP;
  obj->data = malloc(datatype_size(type) * obj->cap);
  assert(obj->data);

  return obj;
}

void darray_free(darray_t* obj) {
  free(obj->data);
  free(obj);
}

datatype_t darray_type(darray_t* obj) {
  return obj->type;
}

size_t darray_size(darray_t* obj) {
  return obj->size;
}

static void* offset_bytes(void* ptr, size_t nbytes) {
  char* bp = ptr;
  return bp + nbytes;
}

void darray_add(darray_t* obj, void* elem) {
  if (obj->size == obj->cap) {
    obj->cap *= 2;
    obj->data = realloc(obj->data, datatype_size(obj->type) * obj->cap);
  }

  memcpy(darray_get(obj, obj->size), elem, datatype_size(obj->type));
  obj->size++;
}

void* darray_get(darray_t* obj, size_t index) {
  assert(index >= 0 && index < obj->cap);
  return offset_bytes(obj->data, index * datatype_size(obj->type));
}

list_it* darray_init() {
  list_it* it = malloc(sizeof(list_it));
  assert(it);

  it->alloc = (void* (*)(datatype_t)) darray_alloc;
  it->free = (void (*)(void*)) darray_free;
  it->type = (datatype_t (*)(void*)) darray_type;
  it->size = (size_t (*)(void*)) darray_size;
  it->add = (void (*)(void*, void*)) darray_add;
  it->get = (void* (*)(void*, size_t)) darray_get;

  return it;
}

