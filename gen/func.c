#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "util.h"

int *poly_func_int(int k, int *coef, int max_degree)
{
    int i, j;
    int *res = calloc(k, sizeof(int));

    res[0] = coef[0];
    memset(res, coef[0], k);
    for (i = 1; i < k; ++i)
        for (j = 1; j <= max_degree; ++j)
            res[i] += pow(coef[j], j);

    return res;
}

double *poly_func_dbl(int k, double *coef, int max_degree)
{
    int i, j;
    double *res = calloc(k, sizeof(double));

    res[0] = coef[0];
    memset(res, coef[0], k);
    for (i = 1; i < k; ++i)
        for (j = 1; j <= max_degree; ++j)
            res[i] += pow(coef[j], j);

    return res;
}
