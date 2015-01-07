#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../bintree.h"

#define SIZE 1024
#define NUM 100

int intcmp(void* ap, void* bp) {
  int* a = (int*) ap;
  int* b = (int*) bp;

  return *a - *b;
}

int main(int argc, char** argv) {
  // make tree and free
  bintree* tree1 = bintree_new(intcmp);
  bintree_free(tree1, free, free);

  // random fuzz testing
  for (int test = 0; test < NUM; test++) {
    // generate random permutation of 0-255
    int contents[SIZE];
    for (int i = 0; i < SIZE; i++)
      contents[i] = i;
    for (int i = 0; i < SIZE; i++) {
      // select random unscrambled item
      int idx = i + rand() % (SIZE - i);

      // swap it with this item
      int swap = contents[i];
      contents[i] = contents[idx];
      contents[idx] = swap;
    }

    // insert these into the binary tree
    bintree* tree = bintree_new(intcmp);
    for (int i = 0; i < SIZE; i++) {
      int* key = (int*) malloc(sizeof(int));
      *key = contents[i];
      int* val = (int*) malloc(sizeof(int));
      *val = i;

      bintree_put(tree, key, val);
    }

    // read back to ensure correctness
    for (int i = 0; i < SIZE; i++) {
      int* val = (int*) bintree_get(tree, &contents[i]);
      assert(*val == i);
    }

    // free contents
    bintree_free(tree, free, free);
  }

  // overwrite testing
  for (int test = 0; test < NUM; test++) {
    // record items in slots
    int contents[SIZE];
    for (int i = 0; i < SIZE; i++)
      contents[i] = -1;

    // add items to random indices
    bintree* tree = bintree_new(intcmp);
    for (int i = 0; i < SIZE; i++) {
      int* key = (int*) malloc(sizeof(int));
      *key = rand();
      int* val = (int*) malloc(sizeof(int));
      *val = i;

      bintree_put(tree, key, val);
      contents[i] = *key;
    }

    // read back
    for (int i = 0; i < SIZE; i++) {
      int* val = (int*) bintree_get(tree, &contents[i]);
      assert(*val == i);
    }

    // free contents
    bintree_free(tree, free, free);
  }
}
