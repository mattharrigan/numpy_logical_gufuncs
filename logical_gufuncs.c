#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "Python.h"
#include "math.h"
#include "numpy/ndarraytypes.h"
#include "numpy/ufuncobject.h"
#include "numpy/npy_3kcompat.h"


/*
 * single_type_logit.c
 * This is the C code for creating your own
 * Numpy ufunc for a logit function.
 *
 * In this code we only define the ufunc for
 * a single dtype. The computations that must
 * be replaced to create a ufunc for
 * a different funciton are marked with BEGIN
 * and END.
 *
 * Details explaining the Python-C API can be found under
 * 'Extending and Embedding' and 'Python/C API' at
 * docs.python.org .
 */

static PyMethodDef LogicalGufuncMethods[] = {
        {NULL, NULL, 0, NULL}
};

/* The loop definition must precede the PyMODINIT_FUNC. */

static void double_all_equal(char **args, npy_intp *dimensions,
                            npy_intp* steps, void* data)
{
    npy_intp i, n;
    npy_intp N = dimensions[0], I = dimensions[1];
    char *a_n = args[0], *b_n = args[1], *c_n = args[2];
    npy_intp a_N = steps[0], b_N = steps[1], c_N = steps[2], a_I = steps[3], b_I = steps[4];

    char *a_i, *b_i;
    npy_double a, b;

    for (n = 0; n < N; n++) {
        a_i = a_n;
        b_i = b_n;
        for (i = 0; i < I; i++){

            a = *(npy_double *)a_i;
            b = *(npy_double *)b_i;
            *((npy_bool *)c_n) = NPY_TRUE;

            if (a == b){
                a_i += a_I;
                b_i += b_I;
            } else {
                *((npy_bool *)c_n) = NPY_FALSE;
                break;
            }
        }

        a_n += a_N;
        b_n += b_N;
        c_n += c_N;
    }
}

/*This a pointer to the above function*/
PyUFuncGenericFunction funcs[1] = {&double_all_equal};

/* These are the input and return dtypes of logit.*/
static char types[3] = {NPY_DOUBLE, NPY_DOUBLE, NPY_BOOL};

static void *data[1] = {NULL};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "logical_gufuncs",
    NULL,
    -1,
    LogicalGufuncMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_logical_gufuncs(void)
{
    PyObject *m, *all_equal, *d;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }

    import_array();
    import_umath();

    all_equal = PyUFunc_FromFuncAndDataAndSignature(funcs, data, types, 1, 2, 1,
                                    PyUFunc_None, "all_equal",
                                    "all_equal_docstring", 0,
                                    "(i),(i)->()");

    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "all_equal", all_equal);
    Py_DECREF(all_equal);

    return m;
}
#else
PyMODINIT_FUNC initlogical_gufuncs(void)
{
    PyObject *m, *all_equal, *d;


    m = Py_InitModule("all_equal", LogicalGufuncMethods);
    if (m == NULL) {
        return;
    }

    import_array();
    import_umath();

    all_equal = PyUFunc_FromFuncAndDataAndSignature(funcs, data, types, 1, 2, 1,
                                    PyUFunc_None, "all_equal",
                                    "all_equal_docstring", 0,
                                    "(i),(i)->()");

    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "all_equal", all_equal);
    Py_DECREF(all_equal);
}
#endif