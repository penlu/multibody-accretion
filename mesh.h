typedef struct mesh mesh;

typedef struct body_list {
  body body;
  struct body_list* next;
} body_list;

mesh* mesh_new(double size);
void mesh_put(mesh* m, body b);
body_list* mesh_get(mesh* m, vector p, int rad);
void mesh_free(mesh* m);
