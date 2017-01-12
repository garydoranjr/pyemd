/*
 * Test EMD Implementation
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <emd.h>

double **array_malloc(int rows, int cols) {
    double **X;
    int i;
    X = (double **) malloc(rows*sizeof(double *));
    for (i = 0; i < rows; i++) {
        X[i] = (double *) malloc(cols*sizeof(double));
    }
    return X;
}

void array_free(double **X, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        free(X[i]);
    }
    free(X);
}

double **euclidean(int rx, int cx, double **X,
                 int ry, int cy, double **Y) {

    if (cx != cy) {
        return NULL;
    }

    double **D;
    double accumulator, diff;
    int i, j, c;

    D = array_malloc(rx, ry);

    for (i = 0; i < rx; i++) {
        for (j = 0; j < ry; j++) {
            accumulator = 0.0;
            for (c = 0; c < cx; c++) {
                diff = X[i][c] - Y[j][c];
                accumulator += diff*diff;
            }
            D[i][j] = sqrt(accumulator);
        }
    }

    return D;
}

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

    double **C;
    C = euclidean(2, 2, X, 2, 2, Y);
    if (C == NULL) {
        return EXIT_FAILURE;
    }

    double *wx;
    double *wy;
    wx = vector_malloc(2);
    wy = vector_malloc(2);
    wx[0] = 0.5;
    wx[1] = 0.5;
    wy[0] = 0.5;
    wy[1] = 0.5;

    double dist;
    dist = emd(2, wx, 2, wy, C, NULL);
    printf("EMD: %f\n", dist);

    free(wx);
    free(wy);
    array_free(C, 2);
    array_free(X, 2);
    array_free(Y, 2);
    return EXIT_SUCCESS;
}
