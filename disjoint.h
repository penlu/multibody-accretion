typedef struct set set;

set* make_set(int i);
set* find(set* s);
void merge(set* s, set* t);
int get_value(set* s);
