/**************************/
/* Python Wrapper for EMD */
/**************************/
#include <stdio.h>

#include <Python.h>
#include <numpy/arrayobject.h>

#include <emd.h>

double **pymatrix_to_Carrayptrs(PyArrayObject *arrayin);
double **ptrvector(long n);
void free_Carrayptrs(double **v);

static PyObject *_emd(PyObject *self, PyObject *args) {
    PyArrayObject *arg1=NULL, *arg2=NULL;
    double **X, **Y;           // The C matrices to be created to point to the 
                                    //   python matrices, cin and cout point to the rows
                                    //   of matin and matout, respectively
    int Xdims[2], Ydims[2];

    printf("Parsing inputs...\n");

    if (!PyArg_ParseTuple(args, "O!O!",
        &PyArray_Type, &arg1,
        &PyArray_Type, &arg2)) {
        return NULL;
    }
    if (NULL == arg1)  return NULL;
    if (NULL == arg2)  return NULL;
    
    printf("Parsing arrays...\n");

    X = pymatrix_to_Carrayptrs(arg1);
    Y = pymatrix_to_Carrayptrs(arg2);

    /* Get the dimensions of the input */
    Xdims[0]=arg1->dimensions[0];
    Xdims[1]=arg1->dimensions[1];
    Ydims[0]=arg2->dimensions[0];
    Ydims[1]=arg2->dimensions[1];

    int i, j;
    double accumulator = 0;
    for ( i=0; i<Xdims[0]; i++) {
        for ( j=0; j<Xdims[1]; j++) {
            accumulator += X[i][j];
        }
    }

    printf("Returning...\n");
    
    free_Carrayptrs(X);
    free_Carrayptrs(Y);

    return Py_BuildValue("d", accumulator);
}

static PyMethodDef pyemd_methods[] = {
   { "_emd", (PyCFunction)_emd, METH_VARARGS,
     "Computes the EMD between two sets of weighted examples." },
   { NULL, NULL, 0, NULL }
};

void initc_emd(void)
{
    Py_InitModule3("c_emd", pyemd_methods,
                   "Earth Mover Distance");
    import_array();
}

/* Utility functions taken from:
*  http://wiki.scipy.org/Cookbook/C_Extensions/NumPy_arrays */

/* ==== Create Carray from PyArray ======================
    Assumes PyArray is contiguous in memory.
    Memory is allocated!                                    */
double **pymatrix_to_Carrayptrs(PyArrayObject *arrayin)  {
    double **c, *a;
    int i,n,m;
    
    n=arrayin->dimensions[0];
    m=arrayin->dimensions[1];
    c=ptrvector(n);
    a=(double *) arrayin->data;  /* pointer to arrayin data as double */
    for ( i=0; i<n; i++)  {
        c[i]=a+i*m;  }
    return c;
}

/* ==== Allocate a double *vector (vec of pointers) ======================
    Memory is Allocated!  See void free_Carray(double ** )                  */
double **ptrvector(long n)  {
    double **v;
    v=(double **)malloc((size_t) (n*sizeof(double)));
    if (!v)   {
        printf("In **ptrvector. Allocation of memory for double array failed.");
        exit(0);  }
    return v;
}

/* ==== Free a double *vector (vec of pointers) ========================== */ 
void free_Carrayptrs(double **v)  {
    free((char*) v);
}
