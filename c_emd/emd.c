/**************************************/
/* EMD - compute earth mover distance */
/**************************************/
#include <stdlib.h>

#include <emd.h>
#include <math.h>

double emd(int n_x, double *weight_x,
           int n_y, double *weight_y, double **cost) {
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

double *vector_malloc(int n) {
    double *v;
    v = (double *) malloc(n*sizeof(double));
    return v;
}

void vector_free(double *v) {
    free(v);
}
