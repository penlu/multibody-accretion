// binary tree node
typedef struct tree {
  void* data;
  tree* l;
  tree* r;
} tree;

tree* newnode(void* data) {
  tree* node = (tree*) malloc(sizeof(tree));
  node->data = data;
  node->l = NULL;
  node->r = NULL;

  return node;
}

// add a piece of data to the tree using given comparison fxn
// returns the new node in which the data is stored
tree* tree_add(tree* root, void* data, int (*cmp)(void* a, void* b)) {
  int res = cmp(data, root->data);
  if (res < 0) {
    // go left
    if (root->l != NULL)
      return tree_add(root->l, data, cmp);
    else
      return root->l = newnode(data);
  } else if (res > 0) {
    // go right
    if (root->r != NULL)
      return tree_add(root->r, data, cmp);
    else
      return root->r = newnode(data);
  } else {
    // data already exists in tree
    return root;
  }
}

// return the node in which a piece of data is stored
// or null if no such node
tree* tree_find(tree* root, void* data, int (*cmp)(void* a, void* b)) {
  // base case (not found)
  if (root == NULL)
    return NULL;

  // otherwise, recurse to find
  int res = cmp(data, root->data);
  if (res < 0)
    return tree_find(root->l, data, cmp);
  else if (res > 0)
    return tree_find(root->r, data, cmp);
  else
    return root;
}

// free a tree, calling the provided function on each piece of data
void tree_free(tree* root, void (*remove)(void* data)) {
  // free children
  tree_free(root->l, remove);
  tree_free(root->r, remove);
  
  // free self
  remove(root->data);
  free(root);
}
