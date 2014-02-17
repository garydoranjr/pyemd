/**************************************/
/* EMD - compute earth mover distance */
/**************************************/
#include <stdlib.h>

#include <emd.h>
#include <math.h>

double emd(double **X, double **Y) {
    return 1.0;
}

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
