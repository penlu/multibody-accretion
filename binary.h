typedef struct btree btree;

btree* btree_new(int (*cmp)(void* a, void* b));
void* btree_get(btree* t, void* key);
void* btree_put(btree* t, void* key, void* val);
void btree_free(btree* t, void (*kfree)(void* key), void (*vfree)(void* val));

