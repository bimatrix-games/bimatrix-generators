#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#ifndef UNIT_GAMES_H
#define UNIT_GAMES_H
/* 
Generates a random no pure unit vector game of size k x k
*/
matrix_t **generate_unit(int k, int is_rand);
#endif
