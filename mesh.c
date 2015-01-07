#include <stdlib.h>

#include "vector.h"
#include "body.h"
#include "mesh.h"
#include "bintree.h"

struct mesh {
  double size;
  bintree* tree;
};

typedef union index {
  struct {
    int x;
    int y;
    int z;
  };
  int e[3];
} index;

int index_cmp(void* ap, void* bp) {
  index* a = (index*) ap;
  index* b = (index*) bp;

  // compare indices in order
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
  return new;
}

index vec_to_idx(vector vec, double size) {
  // floor all the coordinates
  index idx;
  for (int i = 0; i < 3; i++)
    idx.e[i] = (int) (vec.e[i] / size);
  return idx;
}

typedef struct cell {
  body_list* bodies;
} cell;

void mesh_put(mesh* m, body b) {
  // calculate mesh cell index
  index* idx = (index*) malloc(sizeof(idx));
  *idx = vec_to_idx(b.pos, m->size);
  
  // get mesh cell (make a new one if absent)
  cell* c = bintree_get(m->tree, idx);
  if (c == NULL) {
    c = (cell*) malloc(sizeof(cell));
    bintree_put(m->tree, idx, c); // TODO can we eliminate the second tree traversal this entails?
  }
  
  // add new item to mesh
  body_list* new = malloc(sizeof(body_list));
  new->body = b;
  new->next = c->bodies;
  c->bodies = new;
}

// TODO work out return method that doesn't require caller to free
body_list* mesh_get(mesh* m, vector p, int rad) {
  index idx = vec_to_idx(p, m->size);
  index cur = idx;

  body_list* list = NULL;
  for (int i = -rad; i <= rad; i++) {
    cur.e[0] = idx.e[0] + i;
    for (int j = -rad; j <= rad; j++) {
      cur.e[1] = idx.e[1] + j;
      for (int k = -rad; k <= rad; k++) {
        cur.e[2] = idx.e[2] + k;

        cell* c = bintree_get(m->tree, &cur);
        if (c)
          for (body_list* item = c->bodies; item; item = item->next) {
            body_list* append = (body_list*) malloc(sizeof(body_list));
            append->body = item->body;
            append->next = list;
          }
      }
    }
  }

  return list;
}

void cellfree(void* cp) {
  cell* c = (cell*) cp;
  body_list* next = c->bodies;
  for (body_list* cur = next; cur; cur = next) {
    next = cur->next;
    free(cur);
  }

  free(c);
}

// TODO to make a general free, we need to use partial applications
void mesh_free(mesh* m) {
  bintree_free(m->tree, free, cellfree);
}
