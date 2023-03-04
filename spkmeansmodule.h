#ifndef SPKMEANS_MODULE
#define SPKMEANS_MODULE
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "spkmeans.h"
#include "context.h"

static PyObject *spkmeansmodule_spk(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_wam(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_ddg(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_gl(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_jacobi(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_getMatrix(matrix* m);
static vector   *spkmeansmodule_getDataPoints(PyObject*pyDataPoints, context* c);


#endif