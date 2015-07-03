// huffman.c
// Huffman coding
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "huffman.h"

#include <string.h>
#include <assert.h>

#include <hwp_utils/queue.h>

static int compar_weight(sid_t* a, sid_t* b,
    unsigned int* weights) {
  return weights[*a] - weights[*b];
}

htree_t* htree_build(sid_t nsym, unsigned int* weights) {
  assert(nsym > 1);

  htree_t* ret = malloc(sizeof(htree_t));
  assert(ret);

  ret->nsym = nsym;
  ret->parent = malloc(sizeof(sid_t) * (nsym * 2 - 1));
  assert(ret->parent);
  ret->lchild = malloc(sizeof(sid_t) * (nsym * 2 - 1));
  assert(ret->lchild);
  ret->rchild = malloc(sizeof(sid_t) * (nsym * 2 - 1));
  assert(ret->rchild);

  unsigned int* aw = malloc(sizeof(unsigned int) 
      * (nsym * 2 - 1));
  memcpy(aw, weights, sizeof(unsigned int) * nsym);

  pqueue_t* branches = pqueue_alloc(sizeof(sid_t), NULL,
      NULL, (compar_f) compar_weight, aw);
  sid_t i;
  for (i = 0; i < nsym; i++) {
    pqueue_push(branches, &i);
  }

  assert(i == nsym);
  while (pqueue_size(branches) >= 2) {
    sid_t l = VOID_TO_TYPE(pqueue_pop(branches), sid_t);
    sid_t r = VOID_TO_TYPE(pqueue_pop(branches), sid_t);

    ret->parent[l] = i;
    ret->parent[r] = i;
    ret->lchild[i] = l;
    ret->rchild[i] = r;
    aw[i] = aw[l] + aw[r];
    pqueue_push(branches, &i);
    i++;
  }

  assert(i == nsym * 2 - 1);

  free(aw);
  pqueue_free(branches);

  return ret;
}

void htree_free(htree_t* tree) {
  if (tree) {
    free(tree->parent);
    free(tree->lchild);
    free(tree->rchild);
    free(tree);
  }
}

static unsigned int htree_encode_sub(htree_t* tree,
    sid_t b, bitio_t* out) {
  if (b == tree->nsym * 2 - 2) {
    // root
    return 0;
  }
  else {
    sid_t p = tree->parent[b];
    unsigned int l = htree_encode_sub(tree, p, out);
    if (b == tree->lchild[p]) {
      bitio_fputb(0, out);
    }
    else {
      assert(b == tree->rchild[p]);
      bitio_fputb(1, out);
    }
    return l + 1;
  }
}

unsigned int htree_encode(htree_t* tree, sid_t* seq, 
    unsigned int len, bitio_t* out) {
  unsigned int l = 0;
  unsigned int i;
  for (i = 0; i < len; i++) {
    l += htree_encode_sub(tree, seq[i], out);
  }

  return l;
}

static unsigned int htree_decode_sub(htree_t* tree,
    sid_t* s, bitio_t* in) {
  unsigned int l = 0;
  bit b;
   
  *s = tree->nsym * 2 - 2;
  while (*s >= tree->nsym) { 
    b = bitio_fgetb(in);

    if (b == BIT_EOF) {
      break;
    }
    else if (b == 0) {
      *s = tree->lchild[*s];
    }
    else {
      *s = tree->rchild[*s];
    }
    l++;
  }

  return l;
}

unsigned int htree_decode(htree_t* tree, sid_t** seq, 
    unsigned int* len, bitio_t* in) {
  darray_t* res = darray_alloc(sizeof(sid_t), NULL, NULL);
  unsigned int l = 0;

  sid_t s;
  unsigned int m;
  while ((m = htree_decode_sub(tree, &s, in)) > 0) {
    l += m;
    darray_push_back(res, &s);
  }

  *len = darray_size(res);
  *seq = malloc(*len * sizeof(sid_t));
  memcpy(*seq, res->data, *len * sizeof(sid_t));

  darray_free(res);

  return l;
}

