#ifndef FUNC_H
#define FUNC_H

int *rand_step_func_int(int k, int start, int steps);
double *rand_step_func_dbl(int k, double start, double steps);
int *poly_func_int(int k, int *coef, int max_degree);
double *poly_func_dbl(int k, double *coef, int max_degree);

#endif
