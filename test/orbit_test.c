#include <assert.h>

#include "../vector.h"
#include "../body.h"

vector ZERO = {
  .x = 0,
  .y = 0,
  .z = 0
};

int vector_equals(vector a, vector b) {
  for (int i = 0; i < 3; i++)
    if (a.e[i] != b.e[i])
      return 0;
  return 1;
}

int main(int argc, char** argv) {
  double time;
  int n;
  while (fread(&time, 8, 1, stdin)) {
    fread(&n, 4, 1, stdin);
    assert(n == 2);
    body bodies[n];
    for (int i = 0; i < n; i++)
      body_in_binary(&bodies[i], stdin);

    // check properties that should be conserved
    assert(bodies[0].m == 1 &&
           vector_equals(bodies[0].pos, ZERO) &&
           vector_equals(bodies[0].vel, ZERO));
    assert(bodies[1].m == 0 &&
           bodies[1].pos.z == 0 &&
           bodies[1].vel.z == 0);
   
    // output planet pos and vel x/y
    printf("%2.17f,%2.17f,%2.17f,%2.17f,%2.17f\n", time,
           bodies[1].pos.x, bodies[1].pos.y,
           bodies[1].vel.x, bodies[1].vel.y);
  }
}
