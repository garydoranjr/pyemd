/*****************************************************************
 * EMD - Earth Mover's Distance between weighted samples         *
 * Uses the transportation algorithm described at:               *
 * http://www.me.utexas.edu/~jensen/methods/net.pdf/nettrans.pdf *
 * by Gary Doran, 2014                                           *
 *****************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <emd.h>

struct basic_variable **initialize_flow(int n_x, double *weight_x,
                                        int n_y, double *weight_y,
                                        double **cost);
struct basic_variable *init_basic(int row, int col, double flow);
void insert_basic(struct basic_variable **basis, int size,
                  struct basic_variable *node);
void remove_basic(struct basic_variable **basis, int size,
                  struct basic_variable *node);
void reset_current_adj(struct basic_variable **basis, int size);
void destruct_basis(struct basic_variable **basis, int size);

/*
 * Compute EMD between weighted samples, given a pairwise cost matrix
 * @param int n_x : number of samples in X
 * @param double *weight_x : list of weights of samples in X (sums to 1)
 * @param int n_y : number of samples in Y
 * @param double *weight_y : list of weights of samples in Y (sums to 1)
 * @param double **cost : pairwise cost matrix; cost[i][j] holds cost to
 *     "move dirt" from X_i to Y_j
 * @return double : returns EMD between samples
 */
double emd(int n_x, double *weight_x,
           int n_y, double *weight_y,
           double **cost, double **flows) {
    struct basic_variable **basis;
    struct basic_variable *var, *root, *to_remove;
    struct adj_node *adj;
    double *dual_x, *dual_y;
    int *solved_x, *solved_y;
    int i, j, B, min_row, min_col;
    double min_slack, slack, min_flow, sign, distance;
    B = n_x + n_y - 1;

    basis = initialize_flow(n_x, weight_x, n_y, weight_y, cost);

    // Iterate until optimality conditions satisfied
    dual_x = vector_malloc(n_x);
    dual_y = vector_malloc(n_y);
    solved_x = (int *) malloc(n_x*sizeof(int));
    solved_y = (int *) malloc(n_y*sizeof(int));
    while (1) {
        for (i = 0; i < n_x; i++) { solved_x[i] = 0; }
        for (i = 0; i < n_y; i++) { solved_y[i] = 0; }
        reset_current_adj(basis, B);
        var = basis[0];
        dual_x[var->row] = 0.0;
        solved_x[var->row] = 1;
        while (1) {
            var->color = GRAY;
            if (solved_x[var->row]){
                dual_y[var->col] = (cost[var->row][var->col]
                                    - dual_x[var->row]);
                solved_y[var->col] = 1;
            } else if (solved_y[var->col]) {
                dual_x[var->row] = (cost[var->row][var->col]
                                    - dual_y[var->col]);
                solved_x[var->row] = 1;
            } else {
                assert(FALSE);
            }
            for (adj = var->current_adj; adj != NULL; adj = adj->next) {
                if (adj->variable->color == WHITE) { break; }
            }
            if (adj == NULL) {
                var->color = BLACK;
                var = var->back_ptr;
                if (var == NULL) {
                    break;
                }
            } else {
                var->current_adj = adj->next;
                adj->variable->back_ptr = var;
                var = adj->variable;
            }
        }

        // Check for optimality
        min_row = -1;
        min_col = -1;
        min_slack = 0.0;
        for (i = 0; i < n_x; i++) {
            for (j = 0; j < n_y; j++) {
                slack = cost[i][j] - dual_x[i] - dual_y[j];
                if (min_row < 0 || slack < min_slack) {
                    min_row = i;
                    min_col = j;
                    min_slack = slack;
                }
            }
        }
        for (i = 0; i < B; i++) {
            // If the pivot variable is any of the
            // basis variables, then the optimal
            // solution has been found; set
            // min_slack = 0.0 explicitly to avoid
            // floating point issues in comparison.
            if (basis[i]->row == min_row &&
                basis[i]->col == min_col) {
                min_slack = 0.0;
                break;
            }
        }
        if (min_slack >= -EPSILON) { break; }

        // Introduce a new variable
        var = init_basic(min_row, min_col, 0.0);
        insert_basic(basis, B, var);
        root = var;
        reset_current_adj(basis, B + 1);
        while (1) {
            var->color = GRAY;
            for (adj = var->current_adj; adj != NULL; adj = adj->next) {
                if (var->back_ptr != NULL
                    && (var->back_ptr->row == adj->variable->row
                     || var->back_ptr->col == adj->variable->col)) {
                    continue;
                }
                if (adj->variable == root) {
                    // Found a cycle
                    break;
                }
                if (adj->variable->color == WHITE) { break; }
            }
            if (adj == NULL) {
                var->color = BLACK;
                var = var->back_ptr;
                if (var == NULL) {
                    // Couldn't find a cycle
                    assert(FALSE);
                }
            } else {
                if (adj->variable->color == GRAY) {
                    // We found a cycle
                    root->back_ptr = var;
                    break;
                } else {
                    var->current_adj = adj->next;
                    adj->variable->back_ptr = var;
                    var = adj->variable;
                }
            }
        }

        // Find the largest flow that can be subtracted
        sign = -1.0;
        min_flow = 0;
        to_remove = NULL;
        for (var = root->back_ptr; var != root; var = var->back_ptr) {
            if (sign < 0 && (to_remove == NULL || var->flow < min_flow)) {
                min_flow = var->flow;
                to_remove = var;
            }
            sign *= -1.0;
        }

        // Adjust flows
        sign = -1.0;
        root->flow = min_flow;
        for (var = root->back_ptr; var != root; var = var->back_ptr) {
            var->flow += (sign * min_flow);
            sign *= -1.0;
        }

        // Remove the basic variable that went to zero
        remove_basic(basis, B, to_remove);
    }

    distance = 0;
    for (i = 0; i < B; i++) {
        distance += (basis[i]->flow * cost[basis[i]->row][basis[i]->col]);
    }

    if (flows != NULL) {
        // Initialize to zero
        for (i = 0; i < n_x; i++) {
            memset(flows[i], 0, n_y*sizeof(double));
        }
        for (i = 0; i < B; i++) {
            flows[basis[i]->row][basis[i]->col] = basis[i]->flow;
        }
    }

    free(dual_x);
    free(dual_y);
    free(solved_x);
    free(solved_y);
    destruct_basis(basis, B);
    return distance;
}

/*
 * Initialize the basic variables with a feasible solution
 * using the "Northwest Corner Rule"
 */
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

    free(remaining_x);
    free(remaining_y);
    return basis;
}

struct basic_variable *init_basic(int row, int col, double flow) {
    struct basic_variable *var;
    var = (struct basic_variable*)malloc(sizeof(struct basic_variable));
    var->row = row;
    var->col = col;
    var->flow = flow;
    var->adjacency = NULL;
    var->current_adj = NULL;
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

void remove_basic(struct basic_variable **basis, int size,
                  struct basic_variable *node) {
    // Find node in list
    int i;
    for (i = 0; i < size; i++) {
        if (node == basis[i]) { break; }
    }
    assert (i < size);

    // Remove entry from adjacency lists of other nodes
    struct adj_node *a, *o, *last, *next;
    a = node->adjacency;
    while (a != NULL) {
        last = NULL;
        for (o = a->variable->adjacency; o != NULL;
             o = o->next) {
            if (o->variable == node) {
                if (last == NULL) {
                    a->variable->adjacency = o->next;
                } else {
                    last->next = o->next;
                }
                free(o);
                break;
            }
            last = o;
        }

        next = a->next;
        free(a);
        a = next;
    }
    free(basis[i]);

    basis[i] = basis[size];
}

void reset_current_adj(struct basic_variable **basis, int size) {
    int i;
    for (i = 0; i < size; i++) {
        basis[i]->current_adj = basis[i]->adjacency;
        basis[i]->back_ptr = NULL;
        basis[i]->color = WHITE;
    }
}

void destruct_basis(struct basic_variable **basis, int size) {
    int i;
    struct adj_node *adj;
    struct adj_node *next_adj;
    for (i = 0; i < size; i++) {
        adj = basis[i]->adjacency;
        while (adj != NULL) {
            next_adj = adj->next;
            free(adj);
            adj = next_adj;
        }
        free(basis[i]);
    }
    free(basis);
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
