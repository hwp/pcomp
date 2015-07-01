// bitio.h
// bit-wise input/output
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#ifndef BITIO_H_
#define BITIO_H_

#include <stdio.h>

#define BIT_EOF EOF
#define BITIO_BUFFER_SIZE 1024

typedef int bit;

typedef struct {
  FILE* stream;
  char buffer[BITIO_BUFFER_SIZE];
  int bit_offset;
  int bit_avail;
  int byte_buffed;
} bitio_t;

/**
 * attach bit-wise stream to regular byte stream.
 * the stream can be either input or output.
 * the returned stream should be destroyed using bitio_detach();
 */
bitio_t* bitio_attach(FILE* stream);

/**
 * flush data to the attached stream, write an extra byte,
 * which states the length of bit data in the last byte
 * and free the memory of bitio_t*.
 * each file should also end after detached from the bitio,
 * namely call fclose().
 */
void bitio_detach(bitio_t* io);

/**
 * write one bit to the output stream
 *
 * @param b the bit, should be either 0 or 1
 */
void bitio_fputb(bit b, bitio_t* out);

/**
 * get one bit from the input stream
 *
 * @return the bit or BIT_EOF when file end
 */
bit bitio_fgetb(bitio_t* in);

#endif  // BITIO_H_

