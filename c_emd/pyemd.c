/**************************/
/* Python Wrapper for EMD */
/**************************/

#include <emd.h>
#include <Python.h>

static PyObject *_emd(PyObject *self, PyObject *args) {
    Py_RETURN_NONE;
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
}
