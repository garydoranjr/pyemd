/**************************************/
/* EMD - compute earth mover distance */
/**************************************/
#include <stdlib.h>
#include <stdio.h> // TODO: REMOVE AFTER DEBUGGING

#include <emd.h>
#include <math.h>

struct basic_variable **initialize_flow(int n_x, double *weight_x,
                                       int n_y, double *weight_y,
                                       double **cost);
struct basic_variable *init_basic(int row, int col, double flow);
void insert_basic(struct basic_variable **basis, int size,
                  struct basic_variable *node);

double emd(int n_x, double *weight_x,
           int n_y, double *weight_y, double **cost) {
    struct basic_variable **basis;

    // Initialize
    basis = initialize_flow(n_x, weight_x, n_y, weight_y, cost);
    struct adj_node *adj;
    int i;
    for (i = 0; i < n_x + n_y - 1; i++) {
        printf("row: %d, col: %d, flow: %f\n",
            basis[i]->row, basis[i]->col, basis[i]->flow);
        for (adj = basis[i]->adjacency; adj != NULL; adj = adj->next) {
            printf("\tadj: row: %d, col: %d\n", adj->variable->row, adj->variable->col);
        }
    }

    // Iterate until optimality conditions satisfied

    free(basis);
    return 1.0;
}

struct basic_variable **initialize_flow(int n_x, double *weight_x,
                                       int n_y, double *weight_y,
                                       double **cost){
    struct basic_variable **basis;
    struct basic_variable *basic;
    double *remaining_x;
    double *remaining_y;
    int fx, fy, b, B;
    
    b = 0;
    B = n_x + n_y - 1;

    basis = (struct basic_variable**)malloc(
        (B+1)*sizeof(struct basic_variable*));

    remaining_x = vector_copy(weight_x, n_x);
    remaining_y = vector_copy(weight_y, n_y);
    fx = 0;
    fy = 0;
    while (1) {
        if (fx == (n_x - 1)) {
            for ( ; fy < n_y; fy++) {
                basic = init_basic(fx, fy, remaining_y[fy]);
                insert_basic(basis, b, basic);
                b++;
            }
            break;
        }
        if (fy == (n_y - 1)) {
            for ( ; fx < n_x; fx++) {
                basic = init_basic(fx, fy, remaining_x[fx]);
                insert_basic(basis, b, basic);
                b++;
            }
            break;
        }
        if (remaining_x[fx] <= remaining_y[fy]) {
            basic = init_basic(fx, fy, remaining_x[fx]);
            insert_basic(basis, b, basic);
            b++;
            remaining_y[fy] -= remaining_x[fx];
            fx++;
        } else {
            basic = init_basic(fx, fy, remaining_y[fy]);
            insert_basic(basis, b, basic);
            b++;
            remaining_x[fx] -= remaining_y[fy];
            fy++;
        }
    }

    vector_free(remaining_x);
    vector_free(remaining_y);
    return basis;
}

struct basic_variable *init_basic(int row, int col, double flow) {
    struct basic_variable *var;
    var = (struct basic_variable*)malloc(sizeof(struct basic_variable));
    var->row = row;
    var->col = col;
    var->flow = flow;
    var->adjacency = NULL;
    var->back_ptr = NULL;
    var->color = WHITE;
    return var;
}

void insert_basic(struct basic_variable **basis, int size,
                  struct basic_variable *node) {
    struct adj_node *adj;
    int i;
    basis[size] = node;
    for (i = 0; i < size; i++) {
        if (basis[i]->row == node->row ||
            basis[i]->col == node->col) {
            adj = (struct adj_node*)malloc(sizeof(struct adj_node));
            adj->variable = node;
            adj->next = basis[i]->adjacency;
            basis[i]->adjacency = adj;

            adj = (struct adj_node*)malloc(sizeof(struct adj_node));
            adj->variable = basis[i];
            adj->next = node->adjacency;
            node->adjacency = adj;
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

//double vector_sum(double *v, int n) {
//    double sum;
//    int i;
//    sum = 0;
//    for (i = 0; i < n; i++) {
//        sum += v[i];
//    }
//    return sum;
//}
