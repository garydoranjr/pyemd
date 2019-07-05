/**************************/
/* Python Wrapper for EMD */
/**************************/
#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <emd.h>

static PyObject *_emd(PyObject *self, PyObject *args) {
    PyArrayObject *arg1=NULL, *arg2=NULL, *arg3=NULL;
    PyObject *return_flows = NULL;
    double *weight_x, *weight_y, *data_ptr;
    double **cost;
    int n_x, n_y, i;
    double distance;

    // Parse inputs
    if (!PyArg_ParseTuple(args, "O!O!O!O",
        &PyArray_Type, &arg1,
        &PyArray_Type, &arg2,
        &PyArray_Type, &arg3, &return_flows)) {
        return NULL;
    }
    if (arg1 == NULL || arg2 == NULL || arg3 == NULL || return_flows == NULL) {
        return NULL;
    }

    // Convert inputs
    // Assumes array data is contiguous
    n_x = (int) PyArray_DIMS(arg1)[0];
    n_y = (int) PyArray_DIMS(arg2)[0];
    weight_x = (double *)PyArray_DATA(arg1);
    weight_y = (double *)PyArray_DATA(arg2);
    cost = (double **) malloc((size_t) n_x * sizeof(double *));
    data_ptr = (double *)PyArray_DATA(arg3);
    for (i = 0; i < n_x; i++) { cost[i] = data_ptr + i*n_y; }

    double **flows_data_ptr = NULL;
    double *flows_data = NULL;
    if (PyObject_IsTrue(return_flows)) {
        flows_data = (double*) malloc((size_t)(n_x * n_y * sizeof(double)));
        flows_data_ptr = (double**) malloc((size_t)(n_x * sizeof(double*)));
        for (i = 0; i < n_x; i++) { flows_data_ptr[i] = flows_data + i*n_y; }
    }

    distance = emd(n_x, weight_x, n_y, weight_y, cost, flows_data_ptr);

    free(cost);
    PyObject *retval = NULL;
    if (flows_data_ptr) {
        free(flows_data_ptr);
        PyObject *flows = NULL;
        npy_intp dims[2];
        dims[0] = n_x;
        dims[1] = n_y;
        flows = PyArray_SimpleNewFromData(2, dims, NPY_DOUBLE, flows_data);
        PyArray_ENABLEFLAGS((PyArrayObject*)flows, NPY_ARRAY_OWNDATA);
        retval = Py_BuildValue("(dO)", distance, flows);
    } else {
        retval = Py_BuildValue("d", distance);
    }

    return retval;
}

static PyMethodDef c_emd_methods[] = {
   { "_emd", (PyCFunction)_emd, METH_VARARGS,
    "Computes the EMD between two sets of weighted examples, cost matrix.\n"
    "@param weight_x     : NumPy array of weights for first sample of size n\n"
    "@param weight_y     : NumPy array of weights for second sample of size m\n"
    "@param cost         : NumPy array of size n-by-m holding pairwise costs\n"
    "                      to \"move dirt\" between samples\n"
    "@param return_flows : boolean; whether to return the pairwise flow matrix\n"
    "@return : double value corresponding to the EMD between samples, and the\n"
    "          pairwise flow matrix, if specified"
   },
   { NULL, NULL, 0, NULL }
};

static struct PyModuleDef emdmodule = {
    PyModuleDef_HEAD_INIT,
    "c_emd",
    "Earth Mover's Distance",
    0,
    c_emd_methods
};

PyMODINIT_FUNC
PyInit_c_emd(void)
{
    import_array();
    return PyModule_Create(&emdmodule);
}
