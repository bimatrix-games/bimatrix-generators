#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "util.h"

int *create_score(int k, int start, int steps)
{
    int i;
    int *s = malloc(k * sizeof(int));
    
    for (i = 0; i < k; ++i)
        s[i] = start + (i * steps);
    
    return s;
}

int *create_rand_score(int k, int start, int steps)
{
    int i;
    int *s = malloc(k * sizeof(int));
    
    for (i = 0; i < k; ++i)
        s[i] = s[i-1]+ ((randint(steps) ) + 1);
    
    return s;
}

double *create_cost(int k, double start, int steps)
{
    int i;
    double *s = calloc(k, sizeof(double));
    
    s[0] = start;
    for (i = 1; i < k; ++i)
        s[i] = s[i-1] + ((double)(steps) / (k * steps));
    
    return s;
}

double *create_rand_cost(int k, int steps)
{
    int i;
    double *s = calloc(k, sizeof(double));
    
    for (i = 1; i < k; ++i)
        s[i] = s[i-1] + ((double)(randint(steps) + 1) / (k * steps));

    return s;
}

matrix_t **generate_ranking(int k)
{
    matrix_t **game = malloc(2*sizeof(matrix_t *));
    game[0] = matrix_alloc(k, k);
    game[1] = matrix_alloc(k, k);

    int r = rand() % 2;
    int c = r + 1;
    int *s1 = create_rand_score(k, r, 10);
    int *s2 = create_rand_score(k, c, 10);
    //*s2 = *s1;

    double *c1 = create_rand_cost(k, 10);
    double *c2 = create_rand_cost(k, 10);

    int i, j;
    for (i = 0; i < k; ++i){
        for (j = 0; j < k; ++j) {
            if (s1[i] > s2[j]) {
                game[0]->data[i][j] = 1 - c1[i];
                game[1]->data[i][j] = -c2[j];
            } else if (s1[i] < s2[j]) {
                game[1]->data[i][j] = 1 - c2[j];
                game[0]->data[i][j] = -c1[i];
            } else {
                game[0]->data[i][j] = 0.5 - c1[i];
                game[1]->data[i][j] = 0.5 - c2[j];
            }
        }
    }

    free(s1);
    free(s2);

    return game;
}
