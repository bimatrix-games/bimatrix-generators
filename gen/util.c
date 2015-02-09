#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"

/*
Uniformly random int from range [0, n).
Extract from 
http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
*/
int randint(int n)
{
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        // Chop off all of the values that would cause skew...
        long end = RAND_MAX / n; // truncate skew
        assert (end > 0L);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}
