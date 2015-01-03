typedef union vector {
  struct {
    double x;
    double y;
    double z;
  };
  double e[3];
} vector;

vector vec_add(vector a, vector b);
vector vec_sub(vector a, vector b);
vector vec_mul(vector a, double c);
vector vec_div(vector a, double c);
double vec_dot(vector a, vector b);
double dist(vector a, vector b);
