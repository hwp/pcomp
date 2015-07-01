// symbol.h
// symbol functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <stdio.h>
#include <limits.h>

#include <hwp_utils/list.h>
#include <hwp_utils/map.h>

/**
 * type of symbol id
 */
typedef unsigned int sid_t;

#define SID_NOMATCH UINT_MAX

/**
 * a set of symbols
 */
typedef struct {
  /**
   * number of symbols
   */
  sid_t nsym;

  /**
   * texts of symbols
   * element type is wchar_t*
   */
  darray_t* symbols;

  /**
   * indices of symbols
   * key type is wchar_t*
   * value type is sid_t
   */
  hashmap_t* indices;
} symset_t;

/**
 * create an empty symbol set
 */
symset_t* symset_alloc();

/**
 * free a symbol set
 */
void symset_free(symset_t* sym);

/**
 * add a new symbol to the symbol set
 *
 * @param sym the symbol set
 * @param t the new symbol
 *
 * @return the id of the new symbol
 */
sid_t symset_add(symset_t* sym, wchar_t* t);

/**
 * look up the id of a symbol in the symbol set
 *
 * @param sym the symbol set
 * @param t the symbol
 *
 * @return the id of the symbol or SID_NOMATCH if not found
 */
sid_t symset_lookup(symset_t* sym, wchar_t* t);

/**
 * encode a byte stream, i.e. file, into a sequence of 
 * symbols, which are characters.
 *
 * @param file input stream
 * @param sym set of symbols, should be initialized and empty
 * @param text the encoded sequence, memory will be 
 *   allocated and should be freed by calling free()
 * @return the size of the sequence
 */
unsigned int char_encode(FILE* file, symset_t* sym, sid_t** text);

/**
 * encode a byte stream, i.e. file, into a sequence of 
 * symbols, which are words. Words are seperated by spaces
 * and punctuations, i.e. not iswalnum().
 *
 * @param file input stream
 * @param sym set of symbols, should be initialized and empty
 * @param text the encoded sequence, memory will be 
 *   allocated and should be freed by calling free()
 * @return the size of the sequence
 */
unsigned int word_encode(FILE* file, symset_t* sym, sid_t** text);

/**
 * decode a sequence of symbols and write to a byte stream
 *
 * @param file output stream
 * @param sym set of symbols
 * @param text the encoded sequence
 * @param size of the sequence
 * @return number of symbols decoded, should equal to size
 */
unsigned int symbol_decode(FILE* file, symset_t* sym, sid_t* text, unsigned int size);

#endif  // SYMBOL_H_

