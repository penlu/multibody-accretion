#include "vector.h"

vector vec_add(vector a, vector b) {
  vector sum;
  for (int i = 0; i < 3; i++)
    sum.e[i] = a.e[i] + b.e[i];

  return sum;
}

vector vec_sub(vector a, vector b) {
  return vec_add(a, vec_mul(b, -1));
}

vector vec_mul(vector a, double c) {
  vector prod;
  for (int i = 0; i < 3; i++)
    prod.e[i] = a.e[i] * c;

  return prod;
}

vector vec_div(vector a, double c) {
  return vec_mul(a, 1/c);
}

double vec_dot(vector a, vector b) {
  double acc = 0;
  for (int i = 0; i < 3; i++)
    acc += a.e[i] * b.e[i];

  return acc;
}

double dist(vector a, vector b) {
  vector diff = vec_sub(a, b);
  return vec_dot(diff, diff);
}
