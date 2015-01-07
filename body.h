#include <stdio.h>

typedef struct body {
  double m;
  vector pos;
  vector vel;
} body;

void body_out_binary(body* a, FILE* f);
void body_out_readable(body* a, FILE* f);
void body_in_binary(body* a, FILE* f);
