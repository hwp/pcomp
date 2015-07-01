// bitio.c
// bit-wise input/output
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "bitio.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

bitio_t* bitio_attach(FILE* stream) {
  bitio_t* ret = malloc(sizeof(bitio_t));

  ret->stream = stream;
  ret->bit_offset = 0;
  ret->bit_avail = 0;
  ret->byte_buffed = 0;
  memset(ret->buffer, 0, BITIO_BUFFER_SIZE);

  return ret;
}

void bitio_detach(bitio_t* io) {
  if (io->bit_offset > io->bit_avail) {
    // ouput stream 
    char last_byte = io->bit_offset % CHAR_BIT;
    size_t nbytes = io->bit_offset / CHAR_BIT 
      + (last_byte > 0 ? 1 : 0);
    io->buffer[nbytes] = 
      last_byte == 0 ? CHAR_BIT : last_byte;
    size_t r = fwrite(io->buffer, 1, nbytes + 1, io->stream);
    assert(r == nbytes + 1);
  }

  free(io);
}

void bitio_fputb(bit b, bitio_t* out) {
  out->bit_avail = -1;
  
  if (b == 1) {
    int bp = out->bit_offset / CHAR_BIT;
    int bo = out->bit_offset % CHAR_BIT;
    char t = (char) 1 << bo;
    out->buffer[bp] |= t;
  }

  out->bit_offset++;
  if (out->bit_offset == BITIO_BUFFER_SIZE * CHAR_BIT) {
    size_t r = fwrite(out->buffer, 1, BITIO_BUFFER_SIZE, out->stream);
    assert(r == BITIO_BUFFER_SIZE);
    out->bit_offset = 0;
    memset(out->buffer, 0, BITIO_BUFFER_SIZE);
  }
}

bit bitio_fgetb(bitio_t* in) {
  if (in->bit_offset == in->bit_avail) {
    if (in->byte_buffed < 0) {
      return BIT_EOF;
    }
    else if (in->byte_buffed > 0) {
      assert(in->byte_buffed <= 
          BITIO_BUFFER_SIZE - in->byte_buffed);
      memcpy(in->buffer, in->buffer + (BITIO_BUFFER_SIZE - in->byte_buffed), in->byte_buffed);     
    }

    size_t br = fread(in->buffer + in->byte_buffed, 1,
        BITIO_BUFFER_SIZE - in->byte_buffed, in->stream);
    if (br < BITIO_BUFFER_SIZE - in->byte_buffed) {
      if (ferror(in->stream)) {
        perror("fread");
        exit(EXIT_FAILURE);
      }
      else {
        assert(feof(in->stream));
        if (br == 0 && in->byte_buffed == 0) {
          // empty file
          return BIT_EOF;
        }
      }
      // all data read
      assert(in->byte_buffed + (int) br - 1 >= 0);
      char last_byte = in->buffer[in->byte_buffed + br - 1];
      in->bit_offset = 0;
      in->bit_avail = (in->byte_buffed + br - 2) 
        * CHAR_BIT + last_byte;
      assert(in->bit_avail >= 0);
      in->byte_buffed = -1; // set flag
    }
    else {
      assert(br + in->byte_buffed == BITIO_BUFFER_SIZE);
      in->byte_buffed = 2;
      in->bit_offset = 0;
      in->bit_avail = (BITIO_BUFFER_SIZE - in->byte_buffed) 
        * CHAR_BIT;
    }
  }

  if (in->bit_offset < in->bit_avail) {
    int bp = in->bit_offset / CHAR_BIT;
    int bo = in->bit_offset % CHAR_BIT;
    in->bit_offset++;
    return (in->buffer[bp] >> bo) & 1;
  }
  else {
    return BIT_EOF;
  }
}

