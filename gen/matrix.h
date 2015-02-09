#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef MATRIX_H
#define MATRIX_H

typedef struct
{
    double **data;
    int nrows;
    int ncols;
}matrix_t;

matrix_t* matrix_alloc(int nrows, int ncols);
matrix_t* matrix_copy(matrix_t *mat);
void matrix_free(matrix_t *mat);
void matrix_rand(matrix_t *A);
matrix_t* matrix_trans(matrix_t *A);

#endif
