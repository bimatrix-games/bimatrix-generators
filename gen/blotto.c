#include "util.h"
#include "matrix.h"
#include "blotto.h"
#include "partition.h"

matrix_t *generate_payoff_cov(int n, int c)
{
    matrix_t *cov = matrix_alloc(2, 2);

    cov->data[1][0] = cov->data[0][1] = c;
    cov->data[0][0] = cov->data[1][1] = 1;
    matrix_t *payoff = rand_multivariate_multiple(cov, 0, n);
    matrix_free(cov);

    return payoff;
}

int win_battle(int a1, int a2)
{
    if (a1 > a2)
        return 0;
    else if (a2 > a1)
        return 1;
    return rand() % 2;
}

int *win(int *a1, int *a2, int n)
{
    int *wins = malloc(n*sizeof(int));
    int i;

    for (i = 0; i < n; ++i)
        wins[i] = win_battle(a1[i], a2[i]);

    return wins;
}

void get_payoff(matrix_t *val, double* payoffs, int *s1, int *s2, int n)
{
    int *wins = win(s1, s2, n);

    int i;
    for (i = 0; i < n; ++i){
        payoffs[wins[i]] += val->data[i][wins[i]];
    }
}

matrix_t **generate_blotto(int T, int n, int c)
{
    int m = sorted_partitions(T, n);
    matrix_t *vals = generate_payoff_cov(n, c);
    matrix_t **game = malloc(2*sizeof(matrix_t *));
    game[0] = matrix_alloc(m, m);
    game[1] = matrix_alloc(m, m);

    int i, j;
    for (i = 0; i < m; ++i)
        for (j = 0; j < m; ++j) {
            double payoffs[2] = {0, 0};
            get_payoff(vals, payoffs, partitions[i], partitions[j], n);
            game[0]->data[i][j] = payoffs[0];
            game[1]->data[i][j] = payoffs[1];
        }

    matrix_free(vals);
    return game;
}
