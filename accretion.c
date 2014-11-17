#include <stdlib.h>
#include <math.h>

#define NBODIES 1000
#define SOLARM 0.998
#define BODYM ((1 - SOLARM) / NBODIES)
#define PI 3.1415926535897932
#define DENSITY 8000000000
#define RADIUS(MASS) cbrt(MASS/DENSITY)

typedef union vector {
  struct {
    double x;
    double y;
    double z;
  };
  double e[3];
} vector;

double norm(vector a, vector b) {
  double acc = 0;
  for (int i = 0; i < 3; i++) {
    double diff = b.e[i] - a.e[i];
    acc += diff * diff;
  }

  return acc;
}

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
  body bodies[NBODIES];
  init(NBODIES, bodies);

  // begin simulating
  while (1) {
    output(NBODIES, bodies);
    NBODIES = collide(NBODIES, bodies);
    step(NBODIES, bodies);
  }
}

// return a random double in [0, 1)
double random() {
  return (double) rand() / (RAND_MAX + 1);
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
  srand(0xa56c30da);
  
  // generate bodies
  for (int i = 1; i < NBODIES; i++) {
    bodies[i].m = BODYM;
    
    // generate particle at random location in disk
    double angle = random() * 2 * PI;
    double rad = boxmuller();

    // set particle position
    bodies[i].pos.v.x = rad * cos(angle);
    bodies[i].pos.v.y = rad * sin(angle);
    bodies[i].pos.v.z = 0;
    
    // calculate orbital velocity for circular orbit
    double omass = SOLARM + (1 - SOLARM) * erf(rad); // mass inside orbit
    double vel = sqrt(omass / rad);

    // set particle velocity
    bodies[i].vel.v.x = -vel * sin(angle);
    bodies[i].vel.v.y = vel * cos(angle);
    bodies[i].vel.v.z = 0;
  }

  // set sun properties
  bodies[0].m = SOLARM;
  bodies[0].pos.v = {0, 0, 0};
  bodies[0].vel.v = {0, 0, 0};
  // TODO center-of-mass corrections
}

// outputs body information
int step = 0;
void output(int n, body bodies[]) {
  printf("%d bodies at step %d\n", n, step++);
}

// resolves collisions between bodies, returning new body count
int collide(int n, body bodies[]) {
  // find largest object
  double maxrad = bodies[0];
  int maxi = 0;
  for (int i = 0; i < n; i++) {
    double thisrad = RADIUS(bodies[i].m);
    if (thisrad > maxrad) {
      maxrad = thisrad;
      maxi = i;
    }
  }

  // form mesh for collision detection
  // we bucket all of the items into mesh cells so that we only have to search
  // the nearest neighbor cells for possible collisions.
  // mesh cells form an extremely sparse array so we use a treelike data 
  // structure to store them.
  // and by treelike I mean binary tree
}
