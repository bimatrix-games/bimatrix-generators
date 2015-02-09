#include <assert.h>
#include <math.h>
#include "matrix.h"
#include "util.h"

matrix_t* matrix_alloc(int nrows, int ncols)
{
    int i;
    matrix_t *mat;
   
    mat = malloc(sizeof(matrix_t));
    mat->data = (double **)malloc(sizeof(double *) * nrows);

    for (i = 0; i < nrows; ++i)
        mat->data[i] = (double *)calloc(ncols, sizeof(double));

    mat->nrows = nrows;
    mat->ncols = ncols;

    return mat;
}

matrix_t* matrix_copy(matrix_t *mat)
{
    matrix_t *cpy = matrix_alloc(mat->nrows, mat->ncols);
    
    int i, j;
    for (i = 0; i < mat->nrows; ++i) 
        for (j = 0; j < mat->ncols; ++j)
            cpy->data[i][j] = mat->data[i][j];
    
    return cpy;
}

void matrix_free(matrix_t *mat)
{
    int i;
    for (i = 0; i < mat->nrows; ++i)
        free(mat->data[i]);

    free(mat->data);
    free(mat);
}

void matrix_rand(matrix_t *mat)
{
    int i, j;
    for (i = 0; i < mat->nrows; ++i)
        for (j = 0; j < mat->ncols; ++j)
            mat->data[i][j] = ((double)rand() / (double)RAND_MAX);
}

matrix_t* matrix_trans(matrix_t *A)
{
    matrix_t *mat = matrix_alloc(A->ncols, A->nrows);
    
    int i, j;
    for (i = 0; i < mat->nrows; ++i)
    {
        for (j = 0; j < mat->ncols; ++j)
        {
            mat->data[i][j] = A->data[j][i];
        }
    }
    
    return mat;
}