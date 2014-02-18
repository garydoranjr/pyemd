/**************************************/
/* EMD - compute earth mover distance */
/* Header File                        */
/**************************************/

double emd(int n_x, double *weight_x,
	   int n_y, double *weight_y,
           double **cost);

double **array_malloc(int rows, int cols);
void array_free(double **X, int rows);
double *vector_malloc(int n);
void vector_free(double *v);
