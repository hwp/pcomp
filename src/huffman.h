// huffman.h
// Huffman coding
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "symbol.h"
#include "bitio.h"

/**
 * Huffman tree
 */
typedef struct htree_t {
  /**
   * number of symbols
   */
  sid_t nsym;

  /**
   * id of the parent
   */
  sid_t* parent;

  /**
   * id of the left child
   */
  sid_t* lchild;

  /**
   * id of the right child
   */
  sid_t* rchild;
} htree_t;

/**
 * construct a Huffman tree with the given symbol count
 * (the id of the symbols should be from 0 to nsym - 1)
 *
 * @param nsym number of distinct symbols
 * @param weights the number of appearance of each symbol
 * @return the constructed tree, 
 *   the returned pointer should be freed by htree_free()
 */
htree_t* htree_build(sid_t nsym, unsigned int* weights);

/**
 * free a Huffman tree
 */
void htree_free(htree_t* tree);

/**
 * encode a symbol sequence and output to a file
 *
 * @param tree the Huffman tree
 * @param seq the symbol sequence
 * @param len length of the sequence
 * @param out the bit-wise output stream
 *
 * @return number of encoded bits
 */
unsigned int htree_encode(htree_t* tree, sid_t* seq, 
    unsigned int len, bitio_t* out);

/**
 * decode from a file
 *
 * @param tree the Huffman tree
 * @param seq the decoded symbol sequence to be stored
 *   the memory will be allocated by the function 
 *   and should be freed by the user using free().
 * @param len length of the decoded sequence
 * @param out the bit-wise output stream
 *
 * @return number of decoded bits
 */
unsigned int htree_decode(htree_t* tree, sid_t** seq, 
    unsigned int* len, bitio_t* in);

#endif  // HUFFMAN_H_

