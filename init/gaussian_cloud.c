#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../vector.h"
#include "../body.h"

int NBODIES = 1000; // body count
double SOLARM = 0.998; // solar mass
double TOTALM = 1; // total system mass
double BODYM = 0.000002; // small body mass
int HREAD = 0; // human-readable output flag

void init(int n, body bodies[]);

int main(int argc, char** argv) {
  // parse command line arguments
  // TODO handle malformed inputs
  while (argc > 1) {
    if (!strncmp(argv[1], "-h", 2)) {
      HREAD = 1;

      argc--;
      argv++;
    } else if (!strncmp(argv[1], "-n", 2)) {
      if (argc == 1) {
        printf("Need parameter for -n flag\n");
        return 0;
      }

      NBODIES = atoi(argv[2]);

      argc -= 2;
      argv += 2;
    } else if (!strncmp(argv[1], "-s", 2)) {
      if (argc == 1) {
        printf("Need parameter for -s flag\n");
        return 0;
      }

      SOLARM = atof(argv[2]);

      argc -= 2;
      argv += 2;
    } else {
      printf("Unrecognized flag %s\n", argv[1]);
      return 1;
    }
  }

  BODYM = (1 - SOLARM) / NBODIES;

  body bodies[NBODIES];
  init(NBODIES, bodies);

  // produce output
  if (HREAD) {
    printf("%d bodies total\n", NBODIES);
    for (int i = 0; i < NBODIES; i++) {
      printf("body %d:\n", i);
      body_out_readable(&bodies[i], stdout);
    }
  } else {
    fwrite(&NBODIES, 4, 1, stdout);
    for (int i = 0; i < NBODIES; i++)
      body_out_binary(&bodies[i], stdout);
  }
}

// return a random double in [0, 1)
double random() {
  return (double) rand() / RAND_MAX;
}

// box-muller transform generates two independent random variables
// from a gaussian with mean 0, variance 1
// code from http://www.design.caltech.edu/erik/Misc/Gaussian.html
vector gaussian2d() {
  double x1, x2, w, y1, y2;

  do {
    x1 = 2.0 * random() - 1.0;
    x2 = 2.0 * random() - 1.0;
    w = x1 * x1 + x2 * x2;
  } while ( w >= 1.0 );

  w = sqrt( (-2.0 * log( w ) ) / w );
  y1 = x1 * w;
  y2 = x2 * w;

  vector v;
  v.x = y1;
  v.y = y2;
  v.z = 0;

  return v;
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
    vector pos = gaussian2d();
    bodies[i].pos = pos;
    double radsq = vec_dot(pos, pos);
    double angle = atan2(pos.y, pos.x);

    // calculate orbital velocity for circular orbit
    // mass inside orbit follows rayleigh distribution
    double omass = SOLARM + (1 - SOLARM) * (1 - exp(-radsq / 2));
    double vel = sqrt(omass / sqrt(radsq));

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
