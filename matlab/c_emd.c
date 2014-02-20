/**************************/
/* MATLAB Wrapper for EMD */
/**************************/
#include <mex.h>

#include <emd.h>

void checkArgument(const mxArray *arg, int expected_dims) {
    if (!mxIsDouble(arg)) {
        mexErrMsgTxt("Double arrays required.");
    }
    if (mxIsComplex(arg) || mxIsSparse(arg)) {
        mexErrMsgTxt("Complex or sparse arrays not supported.");
    }
    if (mxGetNumberOfDimensions(arg) != expected_dims) {
        mexErrMsgTxt("Invalid dimensionality of input.");
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    int n_x, n_y, i, j;
    double *weight_x, *weight_y, *data_ptr, *row_major;
    double **cost;
    double distance;
    if (nrhs != 3) {
        mexErrMsgTxt("Invalid number of arguments (3 expected).");
    }
    if (nlhs > 1) {
        mexErrMsgTxt("Only 1 output produced.");
    }
    checkArgument(prhs[0], 2);
    checkArgument(prhs[1], 2);
    checkArgument(prhs[2], 2);

    n_x = mxGetM(prhs[2]);
    n_y = mxGetN(prhs[2]);
    weight_x = mxGetPr(prhs[0]);
    weight_y = mxGetPr(prhs[1]);
    data_ptr = mxGetPr(prhs[2]);
    row_major = (double *) malloc(n_x*n_y*sizeof(double));
    cost = (double **) malloc((size_t) n_x * sizeof(double *));
    for (i = 0; i < n_x; i++) { cost[i] = row_major + i*n_y; }
    // Copy data to row-major format
    for (i = 0; i < n_x; i++) {
        for (j = 0; j < n_y; j++) {
            cost[i][j] = data_ptr[i + n_x*j];
        }
    }

    distance = emd(n_x, weight_x, n_y, weight_y, cost);
    plhs[0] = mxCreateDoubleScalar(distance);

    free(cost);
    free(row_major);
    return;
}
