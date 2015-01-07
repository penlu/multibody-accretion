#include <stdlib.h>

#include "../bintree.h"

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
  for (int test = 0; test < 10; test++) {
    
  }
}
