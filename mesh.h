typedef struct mesh mesh;

typedef struct node {
  void* ptr;
  struct node* next;
} node;

mesh* mesh_new(double size);
void mesh_put(mesh* m, vector pos, void* ptr);
node* mesh_get(mesh* m, vector pos, int rad);
void mesh_free(mesh* m, void (*ptrfree)(void* ptr));
