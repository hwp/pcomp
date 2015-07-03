// test_huffman.c
// test huffman encoding and decoding
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

// test_symbols.c
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2015, All rights reserved.

#include "../src/huffman.h"

#include <hwp_utils/test.h>

HT_TEST(test_build, void*) {
  unsigned int nsym = 3;
  unsigned int count[] = {2, 3, 4};

  htree_t* tree = htree_build(nsym, count);
  HT_ASSERT(tree != NULL);
  htree_free(tree);
}

HT_TEST(test_encode_decode, void*) {
  unsigned int nsym = 3;
  unsigned int count[] = {2, 3, 4};
  unsigned int len = 9;
  sid_t seq[] = {0, 1, 2, 0, 1, 2, 1, 2, 2};

  htree_t* tree = htree_build(nsym, count);

  FILE* file = tmpfile();
  bitio_t* io = bitio_attach(file);
  unsigned int nbits = htree_encode(tree, seq, len, io);
  bitio_detach(io);
  
  HT_ASSERT(nbits == 14);

  fseek(file, 0, SEEK_SET);
  io = bitio_attach(file);

  sid_t* dseq = NULL;
  nbits = htree_decode(tree, &dseq, &len, io);
  bitio_detach(io);

  HT_ASSERT(nbits == 14);
  HT_ASSERT(len == 9);
  unsigned int i;
  for (i = 0; i < len; i++) {
    HT_ASSERT(dseq[i] == seq[i]);
  }

  free(dseq);
  htree_free(tree);
  fclose(file);
}

int main(int argc, char** argv) {
  ht_suit_t* suit = ht_suit_alloc(NULL); 

  ht_option_t option;
  ht_get_option(argc, argv, &option);

  ht_add_test(suit, test_build);
  ht_add_test(suit, test_encode_decode);

  int ret = ht_run_suit(suit, &option);
  ht_suit_free(suit);

  if (ret == 0) {
    exit(EXIT_SUCCESS);
  }
  else {
    exit(EXIT_FAILURE);
  }
}

