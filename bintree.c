#include <stdlib.h>

// binary tree node
typedef struct node {
  void* key;
  void* val;
  struct node* l;
  struct node* r;
} node;

typedef struct bintree {
  struct node* root; // root of this tree
  int (*cmp)(void* a, void* b); // key comparison fxn for this tree
} bintree;

bintree* bintree_new(int (*cmp)(void* a, void* b)) {
  bintree* new = (bintree*) malloc(sizeof(bintree));
  new->root = NULL;
  new->cmp = cmp;

  return new;
}

node* new(void* key, void* val) {
  node* new = (node*) malloc(sizeof(node));
  new->key = key;
  new->val = val;
  new->l = NULL;
  new->r = NULL;

  return new;
}

node* put(node* n, void* key, void* val, int (*cmp)(void* a, void* b)) {
  int res = cmp(key, n->key);
  if (res < 0) {
    // go left
    if (n->l != NULL)
      return put(n->l, key, val, cmp);
    else
      return n->l = new(key, val);
  } else if (res > 0) {
    // go right
    if (n->r != NULL)
      return put(n->r, key, val, cmp);
    else
      return n->r = new(key, val);
  } else {
    // this node
    n->val = val;
    return n;
  }
}

// add a piece of data to the tree
void bintree_put(bintree* t, void* key, void* val) {
  if (t->root == NULL)
    t->root = new(key, val);
  else
    put(t->root, key, val, t->cmp);
}

// return value associated with a key
void* get(node* n, void* key, int (*cmp)(void* a, void* b)) {
  // base case (not found)
  if (n == NULL)
    return NULL;

  // otherwise, recurse to find
  int res = cmp(key, n->key);
  if (res < 0)
    return get(n->l, key, cmp);
  else if (res > 0)
    return get(n->r, key, cmp);
  else
    return n->val;
}

// find value associated with key in tree
void* bintree_get(bintree* t, void* key) {
  return get(t->root, key, t->cmp);
}

void nfree(node* n, void (*kfree)(void* key), void (*vfree)(void* val)) {
  if (n != NULL) {
    // free children
    nfree(n->l, kfree, vfree);
    nfree(n->r, kfree, vfree);

    // free self
    kfree(n->key);
    vfree(n->val);
    free(n);
  }
}

// free a tree, calling the provided functions on each key/value
void bintree_free(bintree* t, void (*kfree)(void* key), void (*vfree)(void* val)) {
  nfree(t->root, kfree, vfree);
  free(t);
}

