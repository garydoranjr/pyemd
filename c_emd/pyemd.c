/**************************/
/* Python Wrapper for EMD */
/**************************/
#include <stdio.h>

#include <Python.h>
#include <numpy/arrayobject.h>

#include <emd.h>

static PyObject *_emd(PyObject *self, PyObject *args) {
    PyArrayObject *arg1=NULL, *arg2=NULL, *arg3=NULL;
    double *weight_x, *weight_y, *data_ptr;
    double **cost;
    int n_x, n_y, i;
    double distance;

    printf("Parsing inputs...\n");
    fflush(stdout);

    if (!PyArg_ParseTuple(args, "O!O!O!",
        &PyArray_Type, &arg1,
        &PyArray_Type, &arg2,
        &PyArray_Type, &arg3)) {
        return NULL;
    }
    if (NULL == arg1)  return NULL;
    if (NULL == arg2)  return NULL;
    if (NULL == arg3)  return NULL;
    
    printf("Parsing arrays...\n");

    /* Get the dimensions of the input */
    n_x = arg1->dimensions[0];
    n_y = arg2->dimensions[0];

    weight_x = (double *)arg1->data;
    weight_y = (double *)arg2->data;

    cost = (double **) malloc((size_t) n_x * sizeof(double *));
    data_ptr = (double *)arg3->data;
    for (i = 0; i < n_x; i++) { cost[i] = data_ptr + i*n_y; }

    distance = emd(n_x, weight_x, n_y, weight_y, cost);

    printf("Returning...\n");
    
    free(cost);

    return Py_BuildValue("d", distance);
}

static PyMethodDef c_emd_methods[] = {
   { "_emd", (PyCFunction)_emd, METH_VARARGS,
     "Computes the EMD between two sets of weighted examples." },
   { NULL, NULL, 0, NULL }
};

void initc_emd(void)
{
    Py_InitModule3("c_emd", c_emd_methods,
                   "Earth Mover Distance");
    import_array();
}
