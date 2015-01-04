#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"
#include "disjoint.h"
#include "body.h"

void output(int n, body bodies[]);
int collide(int n, body bodies[]);
void step(int n, body bodies[], double h);

int VERBOSITY = 1;

int main(int argc, char** argv) {
  // parse command line arguments
  FILE* io = stdin;
  while (argc > 2) {
    if (strcmp(argv[1], "-v") == 0) {
      VERBOSITY = atoi(argv[2]);
      
      // next argument
      argc -= 2;
      argv += 2;
    } else {
      printf("Unrecognized argument %s\n", argv[1]);
      return 1;
    }
  }

  // read initial state from input
  int n;
  fread(&n, 4, 1, io);
  
  body bodies[n];
  for (int i = 0; i < n; i++)
    body_in_binary(&bodies[i], io);

  // begin simulating
  while (1) {
    output(n, bodies);
    n = collide(n, bodies);
    step(n, bodies, 0.01);
  }
}

// outputs body information
int generation = 0;
void output(int n, body bodies[]) {
  if (VERBOSITY >= 1)
    printf("%d bodies at step %d\n", n, generation++);

  if (VERBOSITY >= 2)
    // dump information on all bodies
    for (int i = 0; i < n; i++) {
      printf("body %d:\n", i);
      body_out_readable(&bodies[i], stdout);
    }
}

// merge two bodies
body body_merge(body a, body b) {
  body merged;
  merged.m = a.m + b.m;
  merged.pos = vec_add(a.pos, b.pos);
  merged.vel = vec_div(vec_add(vec_mul(a.pos, a.m), vec_mul(a.pos, b.m)), b.m);

  return merged;
}

// resolves collisions between bodies, returning new body count
int collide(int n, body bodies[]) {
  // initialize disjoint set and bodies to include
  set* bsets[n];
  int include[n];
  for (int i = 0; i < n; i++) {
    bsets[i] = make_set(i);
    include[i] = 1;
  }

  // find collisions
  for (int i = 0; i < n; i++) {
    vector ipos = bodies[i].pos;
    double irad = RADIUS(bodies[i].m);

    // which bodies are in contact with this one?
    for (int j = i; j < n; j++) {
      vector jpos = bodies[j].pos;
      double jrad = RADIUS(bodies[j].m);
      
      // merge sets of colliding objects
      if (dist(ipos, jpos) < (irad + jrad) * (irad + jrad))
        merge(bsets[i], bsets[j]);
    }
  }

  // merge objects
  for (int i = 0; i < n; i++) {
    int rootidx = get_value(find(bsets[i]));
    if (rootidx != i) {
      include[i] = 0;
      bodies[rootidx] = body_merge(bodies[rootidx], bodies[i]);
    }
  }

  // copy down
  int j = 0;
  for (int i = 0; i < n; i++) {
    if (include[i])
      bodies[j++] = bodies[i];
  }

  return j;
}

// steps the simulation one timestep
// using a naive integrator; no optimizations
void step(int n, body bodies[], double h) {
  // accumulate accelerations
  for (int i = 0; i < n - 1; i++) {
    body* a = &bodies[i];
    for (int j = i; j < n; j++) {
      body* b = &bodies[j];

      // calculate accelerations due to gravity
      if (a->m != 0 || b->m != 0) {
        double d = sqrt(dist(a->pos, b->pos));
        vector f = vec_mul(vec_sub(a->pos, b->pos), h*d*d*d);

        a->vel = vec_sub(a->vel, vec_mul(f, b->m));
        b->vel = vec_add(b->vel, vec_mul(f, a->m));
      }
    }
  }

  // accumulate velocities
  for (int i = 0; i < n; i++) {
    body* a = &bodies[i];
    a->pos = vec_add(a->pos, vec_mul(a->vel, h));
  }
}
