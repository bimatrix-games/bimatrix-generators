#ifndef UTIL_H
#define UTIL_H
/* Generates an integer uniformly at random from [0, n) */
struct matrix_t;
int randint(int n);
double rand_norm (double mu, double sigma);
struct matrix_t *matrix_multivariate_multiple(struct matrix_t *cov, double mu, int reps);
#endif
