#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"
#include "body.h"
#include "disjoint.h"
#include "mesh.h"

void output(int n, body bodies[], double time);
int collide(int n, body bodies[]);
void step(int n, body bodies[], double h);

int VERBOSITY = 1;
double STEPSIZE = 0.01;

int main(int argc, char** argv) {
  // parse command line arguments
  FILE* infile = stdin;
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

  // last argument is a filename
  if (argc == 2) {
    infile = fopen(argv[1], "r");
    if (infile == NULL) {
      printf("Could not find file %s\n", argv[1]);
      return 1;
    }
  }

  // read initial state from input
  int n;
  fread(&n, 4, 1, infile);
  
  body bodies[n];
  for (int i = 0; i < n; i++)
    body_in_binary(&bodies[i], infile);

  // begin simulating
  double simtime = 0;
  while (1) {
    output(n, bodies, simtime);
    n = collide(n, bodies);
    step(n, bodies, STEPSIZE);
    simtime += STEPSIZE;
  }
}

// outputs body information
int generation = 0;
void output(int n, body bodies[], double time) {
  // write time
  fwrite(&time, 8, 1, stdout);

  // dump body information to stdout
  fwrite(&n, 4, 1, stdout);
  for (int i = 0; i < n; i++)
    body_out_binary(&bodies[i], stdout);

  if (VERBOSITY >= 1)
    fprintf(stderr, "%d bodies at step %d\n", n, generation++);

  if (VERBOSITY >= 2)
    // dump information on all bodies
    for (int i = 0; i < n; i++) {
      fprintf(stderr, "body %d:\n", i);
      body_out_readable(&bodies[i], stderr);
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

  // find largest object
  double maxrad = RADIUS(bodies[0].m);
  for (int i = 0; i < n; i++) {
    double rad = RADIUS(bodies[i].m);
    if (rad > maxrad)
      maxrad = rad;
  }

  // form mesh for collision detection
  mesh* m = mesh_new(maxrad * 2);
  for (int i = 0; i < n; i++)
    mesh_put(m, bodies[i].pos, i);

  // find collisions
  for (int i = 0; i < n; i++) {
    vector ipos = bodies[i].pos;
    double irad = RADIUS(bodies[i].m);

    // which bodies are in contact with this one?
    // look up position in mesh
    body_list* next = mesh_get(m, ipos, 1);
    for (body_list* cur = next; cur; cur = next) {
      // get candidate collider
      int j = cur->index;
      vector jpos = bodies[j].pos;
      double jrad = RADIUS(bodies[j].m);

      // merge sets of colliding objects
      if (dist(ipos, jpos) < (irad + jrad) * (irad + jrad))
        merge(bsets[i], bsets[j]);

      // traverse and free
      next = cur->next;
      free(cur);
    }
  }

  // free the mesh
  mesh_free(m);

  // merge objects
  for (int i = 0; i < n; i++) {
    int rootidx = get_value(find(bsets[i]));
    if (rootidx != i) {
      include[i] = 0;
      bodies[rootidx] = body_merge(bodies[rootidx], bodies[i]);
    }
  }

  // free sets
  for (int i = 0; i < n; i++)
    free(bsets[i]);

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
