#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../matrix.h"
#include "../util.h"

int main(int argc, char **argv)
{
    matrix_t *cov = matrix_alloc(2, 2);
    cov->data[0][1] = cov->data[1][0] = atof(argv[1]);
    cov->data[0][0] = cov->data[1][1] = 1;

    matrix_print(rand_multivariate_multiple(cov, atoi(argv[2]), atoi(argv[3])));

    return 0;
}
