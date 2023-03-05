#ifndef SPKMEANSMODULE
#define SPKMEANSMODULE
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "spkmeans.h"
#include "context.h"
#include "vector.h"
#include "matrix.h"

#define WAM 101
#define DDG 201
#define GL 301

static PyObject *spkmeansmodule_spk(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_wam(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_ddg(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_gl(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_jacobi(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_getPyMatrix(matrix* m);
static vector   *spkmeansmodule_getDataPoints(PyObject*pyDataPoints, context* c);


#endif