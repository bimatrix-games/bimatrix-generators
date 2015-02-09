#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

/*
matrix_t *create_tourn(int k);
int sub_bipartite(matrix_t *graph, int srow, int scol);
int row_sum(matrix_t *graph, int row);
int get_column(int i, int j, int n);
void dominate(matrix_t *tourn, matrix_t *graph, int row);
matrix_t *tourn_bipartite(matrix_t *tourn, int k);
matrix_t **bipartite_game(matrix_t *bipartite);*/

matrix_t **generate_tournament(int n, int k);

#endif
