/*
 * Test EMD Implementation
 */
#include <stdlib.h>
#include <stdio.h>

#include <emd.h>

int main(int argc, char *argv[]) {
    double **X;
    double **Y;
    X = array_malloc(2, 2);
    Y = array_malloc(2, 2);
    X[0][0] =  0.0;
    X[0][1] = 10.0;
    X[1][0] =  1.0;
    X[1][1] =  0.0;
    Y[0][0] =  0.0;
    Y[0][1] = -1.0;
    Y[1][0] = -1.0;
    Y[1][1] =  0.0;

    double dist;
    dist = emd(X, Y);
    printf("EMD: %f\n", dist);

    array_free(X, 2);
    array_free(Y, 2);
    return EXIT_SUCCESS;
}
