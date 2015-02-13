#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../partition.h"

/* Driver program to test above functions */
int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int i;

    int count = sorted_partitions(n, k);
    printf("\n====================\n");
    int j;
    for (i = 0; i < count; ++i){
        for (j = 0; j < k; ++j)
            printf("%d ", partitions[i][j]);
        printf("\n");
    }
    printf("Count %d\n", count);

    return 0;
}
