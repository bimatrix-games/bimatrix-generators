#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "util.h"

/* Creates a random tournament graph */
matrix_t *create_tourn(int k)
{
    matrix_t *tourn = matrix_alloc(k, k);

    int i, j;
    for (i = 0; i < k; ++i){
        for (j = i+1; j < k; ++j) {
            tourn->data[i][j] = rand() % 2;
            tourn->data[j][i] = 1 - tourn->data[i][j];
        }
    }
    return tourn;
}

/*
Returns the sum of the given row
*/
int row_sum(matrix_t *graph, int row)
{
    int i, sum = 0;
    for (i = 0; i < graph->ncols; ++i)
        sum += graph->data[row][i];
    return sum;
}

/*
Computes n choose k
*/
long choose(int n, int k)
{
    double c = 1;
    int i;
    
    for (i = 1; i <= k; ++i) 
        c *= ((double)(n - k + i) / i);
    
    return (int)c;
}

int choose_length(int s, int e, int k)
{
    return choose(e - s + 1, k);
}

/*
Given an array v with k numbers in descending order, compute the
lexicographic index of v in the ordered set of combinations of
[1..n] choose k. Example if n=5, k=3, arrays with their lex-indicies
are
{3, 2, 1} -> 1
{5, 4, 3} -> 10
*/
int comb_index_rev(int *v, int si, int n, int k, int s)
{
    if (k == 1)
        return v[si] - v[si + 1];

    int l = 0;
    int i;
    for (i = s; i <= (n - k); ++i) {
        if (i == v[si])
            break;
        int y = choose_length(i + 1, n, k - 1);
        l += y;
    }
    
    int sp = v[si] + 1;
    int x = comb_index_rev(v, si-1, n, k - 1, sp);
    l+=x;
    return l;
}

int *c;
/*
Fill the arcs from partition R to partition C
Generation of combinations gotten from Rosetta Code
http://rosettacode.org/wiki/Combinations#Lexicographic_ordered_generation
*/
void sub_bipartite_k(matrix_t *graph, int k)
{
    if (c == NULL)
        c = calloc(graph->ncols, sizeof(int));
    
	int i;
    int m = graph->nrows;
    int n = k;
	for (i = 0; i < n; i++) c[i] = n - i;
 
	while (1) {
        int col = comb_index_rev(c, n - 1, m, n, 1)-1;
        for (i = n; i--;){
            graph->data[c[i]-1][col] = 1;
        }
 
		for (i = 0; c[i] >= m - i;){if (++i >= n) return;}
		for (c[i]++; i; i--) c[i-1] = c[i] + 1;
	}
}

/*
For node r in partition R, set all arcs from C to node r
*/
void set_row(matrix_t *tourn, matrix_t *graph, int row, int k)
{
    if (c == NULL)
        c = calloc(graph->ncols, sizeof(int));
    
	int i;
    int m = graph->nrows;
    int n = k;
	for (i = 0; i < n; i++) c[i] = n - i;
 
	while (1) {
        int skip = 0;
        for (i = n; i--;){
            if (c[i] - 1 == row || tourn->data[row][c[i] - 1] == 1){
                skip = 1;
                break;
            }
        }
        if (!skip)
            graph->data[row][comb_index_rev(c, n - 1, m, n, 1)-1] = -1;
 
		for (i = 0; c[i] >= m - i;){if (++i >= n) return;}
		for (c[i]++; i; i--) c[i-1] = c[i] + 1;
	}
}

/*
For node r in partition R, set all arcs from C to node r
*/
void dominate_k(matrix_t *tourn, matrix_t *graph, int row, int k)
{
    int l, zeros;
    
    zeros = tourn->nrows - row_sum(tourn, row) - 1;
    l = 0;
    
    if (zeros > k - 1) {
        set_row(tourn, graph, row, k);
    }
}

/*
Converts tournament graph to the corresponding assymetric
bipartite graph
*/
matrix_t *tourn_bipartite(matrix_t *tourn, int k)
{
    int l = tourn->nrows;
    int m = l;
    int n = choose(m, k);

    matrix_t *graph = matrix_alloc(m, n);
    int i, scol= 0;

    sub_bipartite_k(graph, k);
    for (i = 0; i < tourn->nrows; i++){
        dominate_k(tourn, graph, i, k);
        scol += --l;
    }

    return graph;
}

/*
Converts the given bipartite graph into a win-lose game
*/
matrix_t **bipartite_game(matrix_t *bipartite)
{
    matrix_t **game = malloc(2*sizeof(matrix_t *));
    game[0] = matrix_alloc(bipartite->nrows, bipartite->ncols);
    game[1] = matrix_alloc(bipartite->nrows, bipartite->ncols);

    int i, j;
    for (i = 0; i < bipartite->nrows; ++i) {
        for (j = 0; j < bipartite->ncols; ++j) {
            if (bipartite->data[i][j] == -1)
                game[0]->data[i][j] = 1;
            if (bipartite->data[i][j] == 1)
                game[1]->data[i][j] = 1;
        }
    }
    return game;
}

/*
Creates a tournament game of size n x (n choose k)
*/
matrix_t **generate_tournament(int n, int k)
{
    matrix_t *tourn = create_tourn(n);
    matrix_t *partite = tourn_bipartite(tourn, k);
    matrix_t **game = bipartite_game(partite);
    matrix_free(partite);
    matrix_free(tourn);

    return game;
}
