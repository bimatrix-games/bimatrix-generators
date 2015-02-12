#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "util.h"

/*
Generates a matrix where cells with 1's correspond to best responses
for each of the row players strategy 
*/
matrix_t *best_response_matrix(matrix_t *B)
{
    matrix_t *best_res = matrix_alloc(B->nrows, B->ncols);
    
    int i;
    for (i = 0; i < B->nrows; ++i)
    {
        int m = 0;
        int c = 1;
        int j;
        for (j = 1; j < B->ncols; ++j){
            if (B->data[i][j] > B->data[i][m]) {
                m = j;
                c = 1;
            }
            else if(B->data[i][j] == B->data[i][m]){
                c++;
            }
        }
        for (j = m; c >= 1; ++j) {
            if (B->data[i][j] == B->data[i][m]) {
                best_res->data[i][j] = 1;
                c--;
            }
        }
    }
    return best_res;
}

/*
Returns a random row r, where column 'col' is not a best response
to r
*/
int unit_not_br(matrix_t *br, int col)
{
    int i;
    int c = 0;

    for (i = 0; i < br->nrows; ++i)
        c += (int)br->data[i][col];

    int r = (c == br->nrows) ? br->nrows : br->nrows - c;
    int e = randint(r);

    if (c == br->nrows)
        return e;

    int j = 0;
    for (i = 0; j <= e; ++i) {
        //if (A->data[i][col] < A->data[m][col])
        if (br->data[i][col] == 0)
            ++j;
    }

    //printf("Best resp %d %d chose %d %d\n", c, r, e, i);
    return i - 1;
}

/*
Populates matrix A with unit vector columns such that A, B
has no pure nash
*/
void unit_matrix_no_pure(matrix_t *A, matrix_t *br)
{
    int i, j;
    for (i = 0; i < A->ncols; ++i)
    {
        j = unit_not_br(br, i);
        A->data[j][i] = 1;
    }
}

/* Populates the columns of matrix A with random unit column vectors */
void unit_matrix_rand(matrix_t *A)
{
    int i, j;
    for (i = 0; i < A->ncols; ++i)
    {
        j = randint(A->nrows);
        A->data[j][i] = 1;
    }
}

/*
Generates a random no pure unit vector game of size k x k
*/
matrix_t **generate_unit(int k, int is_rand)
{
    matrix_t **game = malloc(2 * sizeof(matrix_t *));

    game[0] = matrix_alloc(k, k);
    game[1] = matrix_alloc(k, k);

    matrix_rand(game[1]);

    if (is_rand) {
        unit_matrix_rand(game[0]);
    } else {
        matrix_t *br = best_response_matrix(game[1]);
        unit_matrix_no_pure(game[0], br);
    }

    return game;
}
