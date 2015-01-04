#include <stdio.h>

#define DENSITY 8000000000 // TODO in AU scale; what was this
#define RADIUS(MASS) cbrt((MASS)/DENSITY)

typedef struct body {
  double m;
  vector pos;
  vector vel;
} body;

void body_out_binary(body* a, FILE* f);
void body_out_readable(body* a, FILE* f);
void body_in_binary(body* a, FILE* f);
