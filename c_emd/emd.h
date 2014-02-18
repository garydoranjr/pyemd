/**************************************/
/* EMD - compute earth mover distance */
/* Header File                        */
/**************************************/

#define EPSILON 1e-15

double emd(int n_x, double *weight_x,
           int n_y, double *weight_y,
           double **cost);

double **array_malloc(int rows, int cols);
void array_free(double **X, int rows);
double *vector_malloc(int n);
double *vector_copy(double *v, int n);
void vector_free(double *v);

double vector_sum(double *v, int n);

struct heap_entry {
    int row;
    int col;
    double value;
};
void heapify(struct heap_entry *heap, int n);
void siftdown(struct heap_entry *heap, int start, int end);
void heapsort_iter(struct heap_entry *heap, int n);
