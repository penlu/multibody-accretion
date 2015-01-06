#include "mesh.h"
#include "vector.h"

struct mesh {
  double size;
  bintree* tree;
}

typedef union index {
  struct {
    int x;
    int y;
    int z;
  };
  int e[3];
} index

int index_cmp(index* a, index* b) {
  for (int i = 0; i < 3; i++) {
    if (a->e[i] < b->e[i])
      return -1;
    else if (a->e[i] > b->e[i])
      return 1;
  }

  // equal
  return 0;
}

mesh* mesh_new(double size) {
  mesh* new = (mesh*) malloc(sizeof(mesh));
  new->size = size;
  new->tree = bintree_new(index_cmp);
}

index vec_to_idx(vector vec, double size) {
  // floor all the coordinates
  index idx;
  for (int i = 0; i < 3; i++)
    idx.e[i] = (int) (vec.e[i] / size);
  return idx;
}

typedef struct cell {
  node* list;
} cell;

typedef struct entry {
  vector pos;
  void* data;
}

void mesh_put(mesh* m, vector pos, void* ptr) {
  // calculate mesh cell index
  index idx = vec_to_idx(pos);
  
  // get mesh cell (make a new one if absent)
  cell* c = bintree_get(m->tree, idx);
  if (c == NULL) {
    c = (cell*) malloc(sizeof(cell));
    bintree_put(m->tree, idx, c); // TODO can we eliminate the second get?
  }
  
  // add new item to mesh
  entry* data = malloc(sizeof(entry));
  entry->pos = pos;
  entry->data = ptr;
  
  node* new = malloc(sizeof(node));
  node->data = ptr;
  node->next = c->list;
  c->list = node;
}

node* mesh_get(mesh* m, vector pos, int rad) {
  cell* c = bintree_get(m->tree, vec_to_idx(pos));
  if (c == NULL)
    return NULL;
  else
    return c->list;
}

void mesh_free(mesh* m, void (*ptrfree)(void* ptr)) {
  
}
