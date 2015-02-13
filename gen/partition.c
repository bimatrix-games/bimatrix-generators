#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partition.h"

int **partitions = NULL;
int gk;
int count = 0;

void add_partition(int *part)
{
    count++;
    int **tmp = realloc(partitions, count * sizeof(int *));
    if (tmp == NULL)
        exit(1);
    partitions = tmp;

    partitions[count - 1] = calloc(gk, sizeof(int));
    memcpy(partitions[count-1], part, gk * sizeof(int));
}

int compare_partition(const void *a, const void *b)
{
    int *p1 = *(int **)a;
    int *p2 = *(int **)b;

    int i;

    for (i = 0; i < gk; ++i) {
        if (p1[i] < p2[i]) return -1;
        if (p1[i] > p2[i]) return 1;
    }

    return 0;
}

void swap (char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void swap_a(int *a, int x, int y)
{
    int tmp = a[x];
    a[x] = a[y];
    a[y] = tmp;
}

/* Function to print permutations of string
 *    This function takes three parameters:
 *       1. String
 *          2. Starting index of the string
 *             3. Ending index of the string. */
void permute(char *a, int i, int n) 
{
    int j; 
    if (i == n)
        return;
    else
    {
        for (j = i; j <= n; j++)
        {
            swap((a+i), (a+j));
            permute(a, i+1, n);
            swap((a+i), (a+j)); //backtrack
        }
    }
} 

int permuteLexically(int* data, int n) {
    int k = n - 2;
    while (data[k] >= data[k + 1]) {
        k--; if (k < 0) {
            return 0;
        }
    }
    int l = n - 1;
    while (data[k] >= data[l]) {
        l--;
    }
    swap_a(data, k, l);
    int length = n - (k + 1);
    int i;
    for (i = 0; i < length / 2; i++) {
        swap_a(data, k + 1 + i, n - i - 1);
    }
    return 1;
}

void partition_func(int n, int k, int l, int d, int *arr, int *perm)
{
    int i;
    if (k < 1)
        return;
    if (k == 1){
        if (n >= l) {
            arr[d] = n;
            memcpy(perm, arr, gk*sizeof(int));
            do {
                add_partition(perm);
            }while(permuteLexically(perm, gk));
        }
        return;
    }
    for (i = l; i < n + 1; i++){
        arr[d] = i;
        partition_func(n-i, k-1, i, d+1, arr, perm);
    }
}

int sorted_partitions(int n, int k)
{
    int *arr = calloc(k, sizeof(int));
    int *perm = calloc(k, sizeof(int));

    gk = k;
    partition_func(n, k, 0, 0, arr, perm);
    qsort(partitions, count, sizeof(int *), compare_partition);

    free(arr);
    free(perm);

    return count;
}
