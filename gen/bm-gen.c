#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "matrix.h"
#include "tournament.h"
#include "ranking.h"
#include "util.h"
#include "unit_games.h"

#define a(i) i - 1
#define b(i) i + (2*k - 2) 
#define c(i) i - 1
#define d(i) i + (2*k - 2)

int prec = 15;

void write_game_to_file(FILE *f, matrix_t *R, matrix_t *C)
{
    int i, j;
    double a, b;

    fprintf(f, "NFG 1 D \"Using bm-gen\nGame Info\"\n");

    fprintf(f, "{ \"P1\" \"P2\"}");

    int m = R->nrows;
    int n = R->ncols;
    fprintf(f,"{ %d %d }\n\n", m, n);

    //char info[20];
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            a =(R->data[j][i]);
            b =(C->data[j][i]);
            fprintf(f,"%.*lf %.*lf ", prec, a, prec, b);
        }
    }
}

/*
Generates SGC games
*/
matrix_t **generate_sgc(int k)
{
    prec = 3;
    matrix_t **game = malloc(2*sizeof(matrix_t*));
    int i, j, jp, jn, m, n;
    m = n = (4 * k) - 1;
    game[0] = matrix_alloc(m, n);
    game[1] = matrix_alloc(m, n);
    matrix_t *A = game[0];
    matrix_t *B = game[1];
    for (i = 1; i < 2*k; i++)
    {
        if (i == 1)
            jn = 2 * k - 1;
        else
            jn = i - 1;

        if (i == 2 * k - 1)
            jp = 1;
        else
            jp = i + 1;
        
        A->data[a(i)][c(jp)] = 0.5;
        B->data[a(i)][c(jp)] = 1.;

        A->data[a(i)][c(jn)] = 1.;
        B->data[a(i)][c(jn)] = 0.5;

        for (j = 1; j <= 2*k; j++)
        {
            if (j != jp && j != jn && j != 2*k)
            {
                A->data[a(i)][c(j)] = 0.75;
                B->data[a(i)][c(j)] = 0.75;
            }

            A->data[a(i)][d(j)] = 0.5;

            if (j != 2*k)
                B->data[b(i)][c(j)] = 0.5;

            if (i % 2 == 1)
                B->data[b(i)][d(i + 1)] = 0.75;
            else
                B->data[b(i)][d(i - 1)] = 0.75;
        }
        A->data[b(i)][d(i)] = 0.75;
    }

    for (j = 1; j < 2*k; j++)
        B->data[b(i)][c(j)] = 0.5;
    
    B->data[b(i)][d(i - 1)] = 0.75;
    A->data[b(i)][d(i)] = 0.75;
    return game;
}

matrix_t **generate_game(char *game, int s, int k)
{
    if(strncmp(game, "SGC", 3) == 0)
        return generate_sgc(s);
    else if (strncmp(game, "Unit", 3) == 0)
        return generate_unit(s);
    else if (strncmp(game, "Tournament", 10) == 0)
        return generate_tournament(s, k);
    else if (strncmp(game, "Ranking", 7) == 0)
        return generate_ranking(s);
    return NULL;
}

int main(int argc, char **argv)
{
    char c;
    char *game;
    int s;
    int k = 2;
    FILE *f;
    srand(time(NULL));
    while ((c = getopt(argc, argv, "s:f:g:r:k:")) != -1)
        switch (c)
        {
            case 'f':
                f = fopen(optarg,"w+");
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'g':
                game = optarg;
                break;
            case 'r':
                srand(atoi(optarg));
                break;
            case 'k':
                k =  atoi(optarg);
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option '-%c.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character '\\x%x.\n",
                            optopt);
                return 1;
            default:
                break;
        }

    matrix_t **g = generate_game(game, s, k);
    matrix_t *A = g[0];
    matrix_t *B = g[1];
    write_game_to_file(f, A, B);

    matrix_free(A);
    matrix_free(B);
    return 0;
}
