#include <stdlib.h>

typedef struct set {
  int val;
  struct set* parent;
  int rank;
} set;

set* make_set(int i) {
  set* new = (set*) malloc(sizeof(set));
  new->val = i;
  new->parent = new;
  new->rank = 0;

  return new;
}

set* find(set* s) {
  if (s->parent != s)
    s->parent = find(s->parent);
  return s->parent;
}

void merge(set* s, set* t) {
  set* sroot = find(s);
  set* troot = find(t);
  if (sroot == troot) return;

  if (sroot->rank < troot->rank)
    sroot->parent = troot;
  else if (sroot->rank > troot->rank)
    troot->parent = sroot;
  else {
    troot->parent = sroot;
    sroot->rank++;
  }
}

int get_value(set* s) {
  return s->val;
}
