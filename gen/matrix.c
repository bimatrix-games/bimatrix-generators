#include <assert.h>
#include <math.h>
#include "matrix.h"

/* Initializes a matrix with all entries being zero */
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

/* Returns a deap copy of the current matrix */
matrix_t* matrix_copy(matrix_t *mat)
{
    matrix_t *cpy = matrix_alloc(mat->nrows, mat->ncols);
    
    int i, j;
    for (i = 0; i < mat->nrows; ++i) 
        for (j = 0; j < mat->ncols; ++j)
            cpy->data[i][j] = mat->data[i][j];
    
    return cpy;
}

/* Frees the memory used to store the matrix */
void matrix_free(matrix_t *mat)
{
    int i;
    for (i = 0; i < mat->nrows; ++i)
        free(mat->data[i]);

    free(mat->data);
    free(mat);
}

/* Returns 1 if both matrices are equal otherwise 0 */
int matrix_is_equal(matrix_t *A, matrix_t *B)
{
    int i, j;
    
    if ((A->nrows != B->nrows) || (A->ncols != B->ncols))
        return 0;
    
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j)
            if (!err_eq(A->data[i][j], B->data[i][j]))
                return 0;
    
    return 1;
}

/* Returns 1 if the matrix has a negative entry otherwise 0 */
int matrix_has_neg(matrix_t *A)
{
    int i, j;
    
    for (i = 0; i < A->nrows; ++i)
    {
        for (j = 0; j < A->ncols; ++j)
        {
            if (A->data[i][j] < 0)
                return 1;
        }
    }
    
    return 0;
}

/* 
 * Fills the diagonal of the matrix with 1. This should only be run on a
 * square matrix.
 */
void matrix_to_identity(matrix_t *mat)
{
    int i;

    assert(mat->nrows == mat->ncols);
    for(i = 0; i < mat->nrows; ++i)
        mat->data[i][i] = 1;
}

/* Reads the matrix row by row from file 'in' */
void matrix_read(matrix_t *mat, FILE *in)
{
    int i, j;
    for (i = 0; i < mat->nrows; ++i)
        for (j = 0; j < mat->ncols; ++j)
            fscanf(in, "%lf", &mat->data[i][j]);
}

/* Fills the matrix with random entries using rand */
void matrix_rand(matrix_t *mat)
{
    int i, j;
    for (i = 0; i < mat->nrows; ++i)
        for (j = 0; j < mat->ncols; ++j)
            mat->data[i][j] = rand();
}

/* Returns the sum of all entries in the matrix */
double matrix_sum(matrix_t *A)
{
    int i, j;
    double s;

    s = 0;
    for (i = 0; i < A->nrows; i++)
        for (j = 0; j < A->ncols; ++j)
            s += A->data[i][j];
    
    return s;
}

/* 
 * Returns a subset of rows in A that were specified in s
 * where s is a column vector with value of 1 for rows in
 * expected subset and 0 otherwise.
 */
matrix_t* matrix_row_sub(matrix_t *A, matrix_t *s)
{
    int i, j, k, m, n;
    
    m = (int)matrix_sum(s);
    n = A->ncols;
    
    matrix_t *sub = matrix_alloc(m, n);
    k = 0;
    for (i = 0; i < A->nrows; ++i)
        if((int)s->data[i][0])
        {
            for(j = 0; j < A->ncols; ++j)
                sub->data[k][j] = A->data[i][j];
            ++k;
        }
    
    return sub;
}

/* 
 * Returns a subset of rows in A that were specified in s
 * where s contains the index of rows to be in the subset
 */
matrix_t* matrix_row_sub_index(matrix_t *A, matrix_t *s)
{
    int i, j, k;
    matrix_t *sub = matrix_alloc(s->nrows, A->ncols);
    k = 0;
    for (i = 0; i < A->nrows; ++i){
        if(i == (int)s->data[k][0]) {
            for (j = 0; j < A->ncols; ++j)
                sub->data[k][j] = A->data[i][j];
            ++k;
            if (k >= s->nrows)
                break;
        }
    }

    return sub;
}

/* 
 * Returns a subset of columns in A that were specified in s
 * where s is a column vector with value of 1 for columns in
 * expected subset and 0 otherwise.
 */
matrix_t* matrix_col_sub(matrix_t *A, matrix_t *s)
{
    int i, j, k, m, n;

    m = A->nrows;
    n = matrix_sum(s);

    matrix_t *sub = matrix_alloc(m, n);
    k = 0;
    for (j = 0; j < A->ncols; ++j)
        if ((int)s->data[j][0]) {
            for (i = 0; i < A->nrows; ++i)
                sub->data[i][k] = A->data[i][j];
            ++k;
        }

    return sub;
}

/* Normalize the entries of matrix A so that they sum up to 1 */
matrix_t* matrix_prob_norm(matrix_t *A)
{
    matrix_t *res;

    double sum = matrix_sum(A);
    /*
    if (sum == 0){
        sum = A->nrows;
        double inv = 1 / sum;
        int i;
        matrix_add_const(A, inv);
    }*/
    double inv = 1/sum;
    res = matrix_mul_const(A, inv);

    return res;
}

/* 
 * Normalize the entries of matrix A such that all entries A_ij are within the
 * range [0, 1]
 */
matrix_t* matrix_norm(matrix_t *A)
{
    double min, max;
    matrix_min_max(A, &min, &max);
    if (min == max)
        return matrix_alloc(A->nrows, A->ncols);
    return matrix_norm_mm(A, min, max);
}

/* 
 * Normalize the entries of matrix A such that all entries A_ij are within the
 * range [0, 1] given the current minimum (min) and maximum (max) entries of
 * the matrix.
 */
matrix_t* matrix_norm_mm(matrix_t *A, double min, double max)
{
    matrix_t *A_min = matrix_add_const(A, -min);
    matrix_t *res = matrix_mul_const(A_min, 1 / (max - min));
    matrix_free(A_min);
    return res;
}

/* Computes the minimum and maximum values of the matrix A */
void matrix_min_max(matrix_t *A, double *min, double *max)
{
    int i, j;
    double m, M;
    m = A->data[0][0];
    M = A->data[0][0];
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j) {
            m = fmin(m, A->data[i][j]);
            M = fmax(M, A->data[i][j]);
        }

    if (min != NULL)
        *min = m;
    if (max != NULL)
        *max = M;
}

/* Returns a matrix equivalent to -A */
matrix_t* matrix_neg(matrix_t *A)
{
    matrix_t *res = matrix_alloc(A->nrows, A->ncols);

    int i, j;
    for (i = 0; i < A->nrows; i++)
        for (j = 0; j < A->ncols; j++)
            res->data[i][j] = -A->data[i][j];
    
    return res;
}

/* Returns the matrix A+B */
matrix_t* matrix_add(matrix_t *A, matrix_t *B)
{
    assert((A->nrows == B->nrows) && (A->ncols == B->ncols));
    matrix_t *mat = matrix_alloc(A->nrows, A->ncols);
    
    int i, j;
    for (i = 0; i < mat->nrows; ++i)
    {
        for (j = 0; j < mat->ncols; ++j)
        {
            mat->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }
    
    return mat;
}

/* Returns the matrix A + c where c is a constant */
matrix_t* matrix_add_const(matrix_t *A, double c)
{
    matrix_t *mat = matrix_alloc(A->nrows, A->ncols);

    int i, j;
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j)
            mat->data[i][j] = A->data[i][j] + c;

    return mat;
}

/* Returns the matrix A * c where c is a constant */
matrix_t* matrix_mul_const(matrix_t *A, double c)
{
    matrix_t *mat = matrix_alloc(A->nrows, A->ncols);

    int i, j;
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j)
            mat->data[i][j] = A->data[i][j] * c;

    return mat;
}

/* Returns the matrix A * B */
matrix_t* matrix_mul(matrix_t *A, matrix_t *B)
{
    assert(A->ncols == B->nrows);
    matrix_t *mat = matrix_alloc(A->nrows, B->ncols);
    
    double mul;
    
    int i, j, k;
    for (i = 0; i < mat->nrows; ++i)
    {
        for (j = 0; j < mat->ncols; ++j)
        {
            for (k = 0; k < A->ncols; ++k)
            {
                mul = A->data[i][k] * B->data[k][j];
                mat->data[i][j] += mul;
            }
        }
    }
    
    return mat;
}

/* Returns the vector A*b where b is a column vector */
matrix_t* matrix_mul_mat_vec(matrix_t *A, matrix_t *b)
{
    assert(A->ncols == b->nrows);
    matrix_t *mat = matrix_alloc(A->nrows, b->ncols);

    int i, j;
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j)
            mat->data[i][0] += A->data[i][j] * b->data[j][0];

    return mat;
}

/* Returns the vector a*B where a is a row vector */
matrix_t* matrix_mul_vec_mat(matrix_t *a, matrix_t *B)
{
    assert(a->ncols == B->nrows);
    matrix_t *mat = matrix_alloc(a->nrows, B->ncols);

    int i, j;
    for (j = 0; j < B->ncols; ++j)
        for (i = 0; i < B->nrows; ++i)
            mat->data[0][j] += a->data[0][i] * B->data[i][j];

    return mat;
}

/* Returns the matrix A - B */
matrix_t* matrix_sub(matrix_t *A, matrix_t *B)
{
    assert((A->nrows == B->nrows) && (A->ncols == B->ncols));
    matrix_t *mat = matrix_alloc(A->nrows, A->ncols);
    
    int i, j;
    for (i = 0; i < A->nrows; ++i)
    {
        for (j = 0; j < A->ncols; ++j)
        {
            mat->data[i][j] = A->data[i][j] - B->data[i][j];
        }
    }
    
    return mat;
}

/* Returns the transpose of A */
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

/* Returns the augmented matrix A|b */
matrix_t* matrix_augment(matrix_t *A, matrix_t *b)
{
    int i, j;
    matrix_t *res = matrix_alloc(A->nrows, A->ncols + b->ncols);
    
    for (i = 0; i < res->nrows; ++i)
    {
        for (j = 0; j < res->ncols; ++j)
        {
            if (j < A->ncols)
            {
                res->data[i][j] = A->data[i][j];
            }
            else
            {
                res->data[i][j] = b->data[i][j - A->ncols];
            }
        }
    }
    
    return res;
}

/* 
 * Augments multiple matrices in the order passed into the function 
 * A_1 | A_2 | ... | A_count
 */
matrix_t* matrix_augment_cols(int count, ...)
{
    va_list ap;
    int i, j, jp, k, rows, cols;
    matrix_t *res, *mat;
    rows = cols = 0;
    va_start(ap, count);
    for (k = 0; k < count; k++){
        mat = va_arg(ap, matrix_t *);
        cols += mat->ncols;
        rows = mat->nrows;
    }
    va_end(ap);

    va_start(ap, count);
    res = matrix_alloc(rows, cols);
    jp = 0;
    for (k = 0; k < count; k++){
        mat = va_arg(ap, matrix_t *);
        for (j = 0; j < mat->ncols; j++){
            for (i = 0; i < mat->nrows; i++)
                res->data[i][jp] = mat->data[i][j];
            jp++;
        }
    }

    return res;
}

/* Augments matrices A and b row-wise */
matrix_t* matrix_augment_row(matrix_t *A, matrix_t *b)
{
    int i, j;
    matrix_t *res = matrix_alloc(A->nrows + b->nrows, A->ncols);
    
    for (i = 0; i < res->nrows; ++i)
    {
        for (j = 0; j < res->ncols; ++j)
        {
            if (i < A->nrows)
            {
                res->data[i][j] = A->data[i][j];
            }
            else
            {
                res->data[i][j] = b->data[i - A->nrows][j];
            }
        }
    }
    
    return res;
}

/* 
 * Augments multiple matrices row-wise in the order they were passed into the
 * function 
 */
matrix_t* matrix_augment_rows(int count, ...)
{
    va_list ap;
    int i, j, ip, k, rows, cols;
    matrix_t *res, *mat;
    rows = cols = 0;
    va_start(ap, count);
    for (k = 0; k < count; k++){
        mat = va_arg(ap, matrix_t *);
        cols = mat->ncols;
        rows += mat->nrows;
    }
    va_end(ap);

    va_start(ap, count);
    res = matrix_alloc(rows, cols);
    ip = 0;
    for (k = 0; k < count; k++){
        mat = va_arg(ap, matrix_t *);
        for (i = 0; i < mat->nrows; i++){
            for (j = 0; j < mat->ncols; j++)
                res->data[ip][j] = mat->data[i][j];
            ip++;
        }
    }

    return res;
}

/* Converts vector p to permutation matrix P */
matrix_t* matrix_permute(matrix_t *p)
{
    matrix_t *P = matrix_alloc(p->nrows, p->nrows);
    int i, j;

    for (j = 0; j < p->nrows; ++j)
    {
        i = (int)(p->data[j][0]);
        P->data[j][i] = 1;
    }

    return P;
}

/* 
 * Perform LU decomposition on matrix 'mat' where p is a vector 
 * representing the permutation matrix P, l and u represent the
 * lower and upper matrices respectively.
 */
void matrix_LU(matrix_t *mat, matrix_t *p, matrix_t *l, matrix_t *u)
{
    int i, j, k, m;
    
    int max;
    double tmp, t1, t2;

    matrix_t *cpy = matrix_copy(mat);
    
    m = cpy->ncols;
    matrix_to_identity(l);
    
    for (i = 0; i < l->nrows; ++i)
    {
        p->data[i][0] = i;
    }
    
    for (k = 0; k < m; ++k)
    {
        max = k;
        for (i = k + 1; i < m; ++i)
        {
            t1 = fabs(cpy->data[max][k]);
            t2 = fabs(cpy->data[i][k]);
            if (t1 < t2 - eps_err)
            {
                max = i;
                double swap = p->data[k][0];
                p->data[k][0] = p->data[max][0];
                p->data[max][0] = swap;
            }
        }
        
        for (j = 0; j < cpy->ncols; ++j)
        {
            double swap = cpy->data[k][j];
            cpy->data[k][j] = cpy->data[max][j];
            cpy->data[max][j] = swap;
        }
        for (i = k + 1; i < cpy->nrows; ++i)
        {
            if (!err_eq(cpy->data[k][k], 0))
                cpy->data[i][k] = cpy->data[i][k] / cpy->data[k][k];
        }
        for (i = k + 1; i < cpy->nrows; ++i)
        {
            for (j = k + 1; j < cpy->ncols; ++j)
            {
                tmp = cpy->data[i][k] * cpy->data[k][j];
                cpy->data[i][j] = cpy->data[i][j] - tmp;
            }
        }
    }
    
    for (i = 0; i < cpy->nrows; ++i)
    {
        for (j = 0; j < cpy->ncols; ++j)
        {
            if (i <= j)
            {
                u->data[i][j] = cpy->data[i][j];
            }
            else
            {
                l->data[i][j] = cpy->data[i][j];
            }
        }
    }
    
    matrix_free(cpy);
}

/* USED ONLY IN REDUCED ROW ECHELON FORM CODE */
void matrix_normalise_row(matrix_t *mat, int r, int c)
{
    int j;
    double d;
    d = mat->data[r][c];
    
    for (j = 0; j < mat->ncols; ++j)
    {
        mat->data[r][j] = mat->data[r][j] / d;
    }
}

/* 
 * Transform a matrix to reduced row echlon form.
 * NOTE: Not tested, Not used
 */
matrix_t* matrix_red_row_echelon(matrix_t *mat)
{
    int i, j, lead, r;
    
    double t1, t2;
    
    lead = 0;
    matrix_t *res = matrix_copy(mat);
    
    for (r = 0; r < res->nrows; ++r)
    {
        if (res->ncols <= lead)
            break;
        
        i = r;
        //while((res->data[i][lead]) == 0)
        while(err_eq(res->data[i][lead], 0))
        {
            i++;
            if (i == res->nrows)
            {
                i = r;
                lead++;
                if (res->ncols == lead)
                    return res;
            }
        }
        
        for (j = 0; j < res->ncols; ++j)
        {
            double swap = res->data[i][j];
            res->data[i][j] = res->data[r][j];
            res->data[r][j] = swap;
        }
        
        if (!err_eq(res->data[r][lead], 0))
            matrix_normalise_row(res, r, lead);
        
        for (i = 0; i < res->nrows; ++i)
        {
            if (i != r)
            {
                t2 = res->data[i][lead];
                for (j = 0; j < res->ncols; ++j)
                {
                    t1 = t2 * res->data[r][j];
                    res->data[i][j] = res->data[i][j] - t1;
                }
            }
        }
        lead++;
    }
    
    return res;
}
/* Outputs the matrix to screen */
void matrix_print(matrix_t *mat)
{
    int i, j;
    
    for (i = 0; i < mat->nrows; ++i)
    {
        for (j = 0; j < mat->ncols; ++j)
        {
            //mpq_out_str(stdout, 10, mat->data[i][j]);
            printf("%.20le ", (mat->data[i][j]));
        }
        printf("\n");
    }
}

/* Count the number of non-zero entries in A */
int matrix_count_non_zero(matrix_t *A)
{
    int count, i, j;

    count = 0;
    for (i = 0; i < A->nrows; ++i)
        for (j = 0; j < A->ncols; ++j)
            if (A->data[i][j] != 0)
                count++;

    return count;
}
