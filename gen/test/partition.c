#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **list = NULL;
int ggk;
int count = 0;

void add_partition(int *part)
{
    count++;
    int **tmp = realloc(list, count * sizeof(int *));
    if (tmp == NULL)
        exit(1);
    list = tmp;

    list[count - 1] = calloc(ggk, sizeof(int));
    memcpy(list[count-1], part, ggk * sizeof(int));
}

int compare_partition(const void *a, const void *b)
{
    int *p1 = *(int **)a;
    int *p2 = *(int **)b;

    int i;

    for (i = 0; i < ggk; ++i) {
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
        printf("%s\n", a);
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
            printf("\n");
            for (i = 0; i < ggk; i++)
                printf("%d ", arr[i]);
            memcpy(perm, arr, ggk*sizeof(int));
            printf("\n>>");
            do {
                add_partition(perm);
                for (i = 0; i < ggk; i++)
                    printf("%d ", perm[i]);
                printf("\n>>");
            }while(permuteLexically(perm, ggk));
        }
        return;
    }
    for (i = l; i < n + 1; i++){
        arr[d] = i;
        partition_func(n-i, k-1, i, d+1, arr, perm);
    }
}
/* Driver program to test above functions */
int main(int argc, char **argv)
{
    //char a[] = "AABC";  
    //permute(a, 0, 2);
    //int i[] = {1, 2, 3, 4};
    //do{
    //    printf("%d %d %d %d\n", i[0], i[1], i[2], i[3]);
    //}while(permuteLexically(i, 4));
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    ggk = k;
    int *arr = calloc(k, sizeof(int));
    int *perm = calloc(k, sizeof(int));
    int i;
    partition_func(n, k, 0, 0, arr, perm);

    qsort(list, count, sizeof(int *), compare_partition);
    printf("\n====================\n");
    int j;
    for (i = 0; i < count; ++i){
        for (j = 0; j < k; ++j)
            printf("%d ", list[i][j]);
        printf("\n");
    }
    printf("Count %d\n", count);

    return 0;
}
