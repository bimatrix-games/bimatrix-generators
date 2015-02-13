#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>

#ifndef MATRIX_H
#define MATRIX_H

#define eps_err DBL_EPSILON

#define err_ge(x, y) (x > (y - eps_err))
#define err_le(x, y) (x < (y + eps_err))
#define err_eq(x, y) (err_ge(x, y) && err_le(x, y))

typedef struct matrix_t
{
    double **data;
    int nrows;
    int ncols;
}matrix_t;

matrix_t* matrix_alloc(int nrows, int ncols);
matrix_t* matrix_copy(matrix_t *mat);
void matrix_free(matrix_t *mat);
void matrix_read(matrix_t *mat, FILE *in);
int matrix_is_equal(matrix_t *A, matrix_t *B);
int matrix_has_neg(matrix_t *A);
double matrix_sum(matrix_t *A);
void matrix_rand(matrix_t *A);
void matrix_rand_norm(matrix_t *m, double mu, double sigma);
matrix_t* matrix_prob_norm(matrix_t *A);
matrix_t* matrix_norm(matrix_t *A);
matrix_t* matrix_norm_mm(matrix_t *A, double min, double max);
void matrix_min_max(matrix_t *A, double *min, double *max);
matrix_t* matrix_neg(matrix_t *A);
matrix_t* matrix_add(matrix_t *A, matrix_t *B);
matrix_t* matrix_add_const(matrix_t *A, double c);
matrix_t* matrix_mul(matrix_t *A, matrix_t *B);
matrix_t* matrix_mul_const(matrix_t *A, double c);
matrix_t* matrix_mul_mat_vec(matrix_t *A, matrix_t *b);
matrix_t* matrix_mul_vec_mat(matrix_t *a, matrix_t *B);
matrix_t* matrix_sub(matrix_t *A, matrix_t *B);
matrix_t* matrix_trans(matrix_t *A);
matrix_t* matrix_augment(matrix_t *A, matrix_t *b);
matrix_t* matrix_augment_cols(int cols, ...);
matrix_t* matrix_augment_row(matrix_t *A, matrix_t *b);
matrix_t* matrix_augment_rows(int cols, ...);
matrix_t* matrix_permute(matrix_t *p);
void matrix_LU(matrix_t *mat, matrix_t *p, matrix_t *l, matrix_t *u);
matrix_t* matrix_row_sub(matrix_t *A, matrix_t *s);
matrix_t* matrix_row_sub_index(matrix_t *A, matrix_t *s);
matrix_t* matrix_col_sub(matrix_t *A, matrix_t *s);
matrix_t* matrix_red_row_echelon(matrix_t *mat);
matrix_t* matrix_cholesky(matrix_t *A);
void matrix_print(matrix_t *mat);

#endif
