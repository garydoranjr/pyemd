/**************************************/
/* EMD - compute earth mover distance */
/**************************************/
#include <stdlib.h>
#include <stdio.h>

#include <emd.h>
#include <math.h>

double **initialize_flow(int n_x, double *weight_x,
                         int n_y, double *weight_y, double **cost);

double emd(int n_x, double *weight_x,
           int n_y, double *weight_y, double **cost) {
    int i, j;
    double **flow;

    // Initialize
    flow = initialize_flow(n_x, weight_x, n_y, weight_y, cost);
    if (flow == NULL) {
        array_free(flow, n_x);
        return -1.0;
    }

    for (i = 0; i < n_x; i++) {
        for (j = 0; j < n_y; j++) {
            printf("%f ", flow[i][j]);
        }
        printf("\n");
    }

    return 1.0;
}

double **initialize_flow(int n_x, double *weight_x,
                         int n_y, double *weight_y, double **cost){
    double **flow;
    double *remaining_x;
    double *remaining_y;
    struct heap_entry *heap;
    int i, j, idx, n;
    double amtx, amty, min_amt;

    n = n_x*n_y;
    flow = array_malloc(n_x, n_y);
    heap = (struct heap_entry *)malloc(n * sizeof(struct heap_entry));
    for (i = 0; i < n_x; i++) {
        for (j = 0; j < n_y; j++) {
            idx = n_y*i + j;
            heap[idx].row = i;
            heap[idx].col = j;
            heap[idx].value = cost[i][j];

            flow[i][j] = 0.0;
        }
    }

    heapify(heap, n);
    remaining_x = vector_copy(weight_x, n_x);
    remaining_y = vector_copy(weight_y, n_y);
    while (1) {
        heapsort_iter(heap, n);
        n--;
        amtx = remaining_x[heap[n].row];
        amty = remaining_x[heap[n].col];
        if (amtx <= amty) {
            min_amt = amtx;
        } else {
            min_amt = amty;
        }

        if (min_amt > EPSILON) {
            remaining_x[heap[n].row] -= min_amt;
            remaining_y[heap[n].col] -= min_amt;
            flow[heap[n].row][heap[n].col] += min_amt;
        }
        if (vector_sum(remaining_x, n_x) < EPSILON) {
            break;
        }
        if (n == 0) {
            goto fail;
        }
    }

    free(heap);
    vector_free(remaining_x);
    vector_free(remaining_y);
    return flow;

fail:
    free(heap);
    array_free(flow, n_x);
    vector_free(remaining_x);
    vector_free(remaining_y);
    return NULL;
}

void heapify(struct heap_entry *heap, int n) {
    int start;
    for (start = (n-2)/2; 0 <= start; start--) {
        siftdown(heap, start, n-1);
    }
}

/*
 * Performs one iteration of the heapsort algorithm on
 * the first n unsorted elements
 */
void heapsort_iter(struct heap_entry *heap, int n) {
    if (n <= 0) return;
    struct heap_entry tmp;
    tmp = heap[0];
    heap[0] = heap[n-1];
    heap[n-1] = tmp;
    siftdown(heap, 0, n-1);
}

void siftdown(struct heap_entry *heap, int start, int end) {
    struct heap_entry tmp;
    int root, swap, left_child, right_child;

    root = start;
    while ((2*root + 1) <= end) {
        left_child = 2*root + 1;
        right_child = left_child + 1;
        swap = root;
        if (heap[swap].value < heap[left_child].value) {
            swap = left_child;
        }
        if (right_child <= end
                && heap[swap].value < heap[right_child].value) {
            swap = right_child;
        }
        if (swap != root) {
            tmp = heap[root];
            heap[root] = heap[swap];
            heap[swap] = tmp;
            root = swap;
        } else {
            return;
        }
    }
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

double *vector_copy(double *v, int n) {
    double *copy;
    int i;
    copy = (double *) malloc(n*sizeof(double));
    for (i = 0; i < n; i++) {
        copy[i] = v[i];
    }
    return copy;
}

void vector_free(double *v) {
    free(v);
}

double vector_sum(double *v, int n) {
    double sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum += v[i];
    }
    return sum;
}
