typedef struct mesh mesh;

typedef struct body_list {
  int index;
  struct body_list* next;
} body_list;

mesh* mesh_new(double size);
void mesh_put(mesh* m, vector v, int i);
body_list* mesh_get(mesh* m, vector v, int rad);
void mesh_free(mesh* m);
