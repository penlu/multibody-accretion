typedef struct bintree bintree;

bintree* bintree_new(int (*cmp)(void* a, void* b));
void* bintree_get(bintree* t, void* key);
void* bintree_put(bintree* t, void* key, void* val);
void bintree_free(bintree* t, void (*kfree)(void* k), void (*vfree)(void* v));

