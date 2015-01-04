#include <stdio.h>

#include "vector.h"
#include "body.h"

void body_out_binary(body* a, FILE* f) {
  fwrite(a, sizeof(body), 1, f);
}

void body_out_readable(body* a, FILE* f) {
  fprintf(f, "mass: %f\n", a->m);
  fprintf(f, "pos:\n  x:%f\n  y:%f\n  z:%f\n", a->pos.x, a->pos.y, a->pos.z);
  fprintf(f, "vel:\n  x:%f\n  y:%f\n  z:%f\n", a->vel.x, a->vel.y, a->vel.z);
  fprintf(f, "\n");
}

// TODO handle io errors
void body_in_binary(body* a, FILE* f) {
  fread(a, sizeof(body), 1, f);
}
