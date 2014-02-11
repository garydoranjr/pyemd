/**************************/
/* Python Wrapper for EMD */
/**************************/

#include <emd.h>
#include <Python.h>

static PyObject *pyemd(PyObject *self, PyObject *args) {
    Py_RETURN_NONE;
}

static PyMethodDef pyemd_methods[] = {
   { "emd", (PyCFunction)pyemd, METH_VARARGS,
     "Computes the EMD between two sets of weighted examples." },
   { NULL, NULL, 0, NULL }
};

void initemd(void)
{
    Py_InitModule3("emd", pyemd_methods,
                   "Earth Mover Distance");
}
