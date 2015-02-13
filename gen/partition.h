#ifndef PARTITION_H
#define PARTITION_H

extern int **partitions;

void partition_func(int n, int k, int l, int d, int *arr, int *perm);
int sorted_partitions(int n, int k);
#endif
