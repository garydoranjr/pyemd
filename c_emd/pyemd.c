/**************************/
/* Python Wrapper for EMD */
/**************************/
#include <Python.h>

#include <emd.h>

static PyObject *_emd(PyObject *self, PyObject *args) {
    double a;
    double b;
    
    if (!PyArg_ParseTuple(args, "dd", &a, &b)) {
      return NULL;
    }

    return Py_BuildValue("d", emd(a, b));
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
