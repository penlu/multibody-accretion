#include <stdio.h>

#include "../vector.h"
#include "../body.h"

int main(int argc, char** argv) {
  body star;
  star.m = 1;
  star.pos.x = 0; star.pos.y = 0; star.pos.z = 0;
  star.vel.x = 0; star.vel.y = 0; star.vel.z = 0;

  body planet;
  planet.m = 0;
  planet.pos.x = 1; planet.pos.y = 0; planet.pos.z = 0;
  planet.vel.x = 0; planet.vel.y = 1; planet.vel.z = 0;

  int n = 2;
  fwrite(&n, 4, 1, stdout);
  body_out_binary(&star, stdout);
  body_out_binary(&planet, stdout);
}
