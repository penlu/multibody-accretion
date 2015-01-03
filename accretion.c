#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "disjoint.h"

#define NBODIES 1000
#define SOLARM 0.998
#define BODYM ((1 - SOLARM) / NBODIES)
#define PI 3.1415926535897932
#define DENSITY 8000000000 // TODO in AU scale; what was this
#define RADIUS(MASS) cbrt((MASS)/DENSITY)

typedef struct body {
  double m;
  vector pos;
  vector vel;
} body;

void init(int n, body bodies[]);
void output(int n, body bodies[]);
int collide(int n, body bodies[]);
void step(int n, body bodies[]);

int main(int argc, char* argv[]) {
  // create initial state
  int n = NBODIES;
  body bodies[n];
  init(n, bodies);

  // begin simulating
  while (1) {
    output(n, bodies);
    n = collide(n, bodies);
    step(n, bodies, 0.01);
  }
}

// return a random double in [0, 1)
double random() {
  return (double) rand() / RAND_MAX;
}

// box-muller transform generates a random variable from a gaussian
// with mean 0, variance 1
// code from http://www.design.caltech.edu/erik/Misc/Gaussian.html
double boxmuller() {
  double x1, x2, w, y1, y2;

  do {
    x1 = 2.0 * random() - 1.0;
    x2 = 2.0 * random() - 1.0;
    w = x1 * x1 + x2 * x2;
  } while ( w >= 1.0 );

  w = sqrt( (-2.0 * log( w ) ) / w );
  y1 = x1 * w;
  y2 = x2 * w;

  return y1; // TODO use both perfectly good values
}

// initializes a state with n bodies
// this is a coplanar state with a central sun surrounded by a swarm of 
// protoplanetary bodies
void init(int n, body bodies[]) {
  srand(0xa56c30da); // TODO temp random bytes from random.org
  
  // generate bodies
  for (int i = 1; i < NBODIES; i++) {
    bodies[i].m = BODYM;
    
    // generate particle at random location in disk
    double angle = random() * 2 * PI;
    double rad = boxmuller();

    // set particle position
    bodies[i].pos.x = rad * cos(angle);
    bodies[i].pos.y = rad * sin(angle);
    bodies[i].pos.z = 0;
    
    // calculate orbital velocity for circular orbit
    double omass = SOLARM + (1 - SOLARM) * erf(rad); // mass inside orbit
    double vel = sqrt(omass / rad);

    // set particle velocity
    bodies[i].vel.x = -vel * sin(angle);
    bodies[i].vel.y = vel * cos(angle);
    bodies[i].vel.z = 0;
  }

  // set sun properties
  bodies[0].m = SOLARM;
  vector zero = {
    .x = 0,
    .y = 0,
    .z = 0
  };
  bodies[0].pos = zero;
  bodies[0].vel = zero;
  // TODO center-of-mass corrections
}

// outputs body information
int generation = 0;
void output(int n, body bodies[]) {
  printf("%d bodies at step %d\n", n, generation++);
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
  for (int i = 0; i < n - 1; i++)
    for (int j = i; j < n; j++) {
      body a = bodies[i];
      body b = bodies[j];

      // calculate accelerations due to gravity
      double d = sqrt(dist(a.pos, b.pos));
      vector f = vec_mul(vec_sub(a.pos, b.pos), h*d*d*d);

      a.vel = vec_sub(a.vel, vec_mul(f, b.m));
      b.vel = vec_add(b.vel, vec_mul(f, a.m));
    }

  // accumulate velocities
  for (int i = 0; i < n; i++) {
    body a = bodies[i];
    a.pos = vec_add(a.pos, vec_mul(a.vel, h));
  }
}
