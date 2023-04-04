#define PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeanslib.h"
#include "spkmeans.h"

static vector   *spkmeansmodule_getDataPoints(PyObject*pyDataPoints, context* c);

static int spkmeansmodule_isListOfLists_old(PyObject *dataPointsPy);
static vector *spkmeansmodule_getDataPoints(PyObject *pyDataPoints, context *c);
static matrix *spkmeansmodule_getCMatrix(PyObject *pyMatrix);
static ClusterPoint * spkmeansmodule_getDataPoints_old(PyObject*dataPointsPy);
static Centroid * spkmeansmodule_getInitialCentroids(PyObject*initialCentroidsPy);
static int spkmeansmodule_isListOfLists(PyObject*dataPointsPy);

static PyObject *spkmeansmodule_wam(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_ddg(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_gl(PyObject *self, PyObject *args);
static PyObject *spkmeansmodule_jacobi(PyObject *self, PyObject *args);

static PyObject *spkmeansmodule_getPyMatrix(matrix* m);
static PyObject *spkmeansmodule_getList(Centroid*updatedCentroids,int k,int dim);
static PyObject *spkmeansmodule_reverseSwitch(PyObject *args, int how);
static PyObject *spkmeansmodule_getPyMatrix(matrix *m);

static PyObject *spkmeansmodule_reverseSwitch(PyObject *args, int how)
{
    vector *vecs;
    context *c;
    matrix *wam, *ddg, *gl;
    PyObject *dataPointsList;
    PyObject *ret;

    c = (context *)malloc(sizeof(context));
    if (c == NULL)
    {
        return NULL;
    }
    if (!PyArg_ParseTuple(args, "O", &dataPointsList))
    {
        free(c);
        return NULL;
    }
    if ((vecs = spkmeansmodule_getDataPoints(dataPointsList, c)) == NULL)
    {
        free(c);
        return NULL;
    }
    wam = spkmeans_wam(c, vecs);
    vector_destroy(&vecs,1);
    if (wam == NULL)
    {
        free(c);
        return NULL;
    }
    if (how == WAM)
    {
        ret = spkmeansmodule_getPyMatrix(wam);
        free(c);
        matrix_destroy(wam);
        return ret;
    }
    ddg = spkmeans_ddg(wam);
    if (ddg == NULL)
    {
        free(c);
        matrix_destroy(wam);
        return NULL;
    }
    if (how == DDG)
    {
        ret = spkmeansmodule_getPyMatrix(ddg);
        free(c);
        matrix_destroy(wam);
        matrix_destroy(ddg);
        return ret;
    }
    gl = spkmeans_gl(wam, ddg);
    matrix_destroy(wam);
    matrix_destroy(ddg);
    if (gl == NULL)
    {
        free(c);
        return NULL;
    }
    if (how == GL)
    {
        ret = spkmeansmodule_getPyMatrix(gl);
        free(c);
        matrix_destroy(gl);
        return ret;
    }
    return NULL;
}

static PyObject *spkmeansmodule_getPyMatrix(matrix *m)
{
    PyObject *py_list;
    PyObject *sub_list;
    py_list = PyList_New(m->row);
    unsigned int i;
    unsigned int j;
    for (i = 0; i < m->row; ++i)
    {
        sub_list = PyList_New(m->col);
        PyList_SetItem(py_list, i, sub_list);
        for (j = 0; j < m->col; j++)
        {
            PyList_SetItem(sub_list, j, PyFloat_FromDouble(m->matrix[i][j]));
        }
    }
    return py_list;
}

static int spkmeansmodule_isListOfLists(PyObject *dataPointsPy)
{
    /*
        Check that argument is a list of lists
    */
    if (!PyList_Check(dataPointsPy))
    {
        PyErr_SetString(PyExc_ValueError, "MustSupplyPythonList0");
        return 1;
    }
    if (!PyList_Check(PyList_GetItem(dataPointsPy, 0)))
    {
        PyErr_SetString(PyExc_ValueError, "MustSupplyPythonListOfLists1");
        return 1;
    }
    return 0;
}

static vector *spkmeansmodule_getDataPoints(PyObject *pyDataPoints, context *c)
{

    vector *dataPoints;
    double *temp;
    Py_ssize_t n;
    Py_ssize_t dim;
    Py_ssize_t i, j;

    if (spkmeansmodule_isListOfLists(pyDataPoints))
    {
        return NULL;
    }
    /*
        Allocate and get values
    */
    n = PyList_Size(pyDataPoints);
    c->datapoint_count = n;
    dim = PyList_Size(PyList_GetItem(pyDataPoints, 0));
    c->dimention = dim;
    dataPoints = (vector *)malloc(sizeof(vector) * n);
    if (dataPoints == NULL)
    {
        PyErr_SetString(PyExc_ValueError, "MallocF0");
        return NULL;
    }
    /*
        Fill data points
    */
    temp = (double *)malloc(sizeof(double) * dim * n);
    if (temp == NULL)
    {
        PyErr_SetString(PyExc_ValueError, "MallocF1");
        return NULL;
    }
    for (i = 0; i < n; ++i)
    {
        dataPoints[i] = temp + i * dim;
        for (j = 0; j < dim; ++j)
        {
            dataPoints[i][j] =
                PyFloat_AsDouble(
                    PyList_GetItem(PyList_GetItem(pyDataPoints, i), j));
        }
    }
    return dataPoints;
}

static matrix *spkmeansmodule_getCMatrix(PyObject *pyMatrix)
{
    matrix *ret = (matrix *)malloc(sizeof(matrix));
    context *c = (context *)malloc(sizeof(context));
    ret->matrix = spkmeansmodule_getDataPoints(pyMatrix, c);
    ret->row = c->datapoint_count;
    ret->col = c->dimention;
    free(c);
    return ret;
}
static PyObject *spkmeansmodule_wam(PyObject *self, PyObject *args)
{
    PyObject *ret;
    if ((ret = spkmeansmodule_reverseSwitch(args, WAM)) == NULL)
    {
        return NULL;
    }
    return ret;
}
static PyObject *spkmeansmodule_ddg(PyObject *self, PyObject *args)
{
    PyObject *ret;
    if ((ret = spkmeansmodule_reverseSwitch(args, DDG)) == NULL)
    {
        return NULL;
    }
    return ret;
}
static PyObject *spkmeansmodule_gl(PyObject *self, PyObject *args)
{
    PyObject *ret;
    if ((ret = spkmeansmodule_reverseSwitch(args, GL)) == NULL)
    {
        return NULL;
    }
    return ret;
}
static PyObject *spkmeansmodule_jacobi(PyObject *self, PyObject *args)
{
    PyObject *inPyMatrix, *ret;
    matrix *inCMatrix;
    matrix* postAlgoCMatrcies[2];
    int i;
    if (!PyArg_ParseTuple(args, "O", &inPyMatrix))
    {
        return NULL;
    }
    inCMatrix = spkmeansmodule_getCMatrix(inPyMatrix);
    spkmeans_jacobi(inCMatrix, postAlgoCMatrcies);
    ret = PyList_New(2);
    for (i = 0; i < 2; ++i)
    {
        PyList_SetItem(ret, i, spkmeansmodule_getPyMatrix(postAlgoCMatrcies[i]));
        matrix_destroy(postAlgoCMatrcies[i]);
    }
    return ret;
}

static PyObject *spkmeansmodule_spk(PyObject *self, PyObject *args) {
    

    ClusterPoint *dataPoints;
    int dataPointsSize;
    Centroid* initialCentroids;
    int centroidsSize;
    int iter;
    int dim;
    double eps;

    PyObject* dataPointsPy;
    PyObject* initialCentroidsPy;
   
  
    if (!PyArg_ParseTuple(args, "OOidi", &dataPointsPy,&initialCentroidsPy,&iter,&eps,&dim)) {
        return NULL;
    }

    Centroid* updatedCentroids;
    dataPointsSize = PyList_Size(dataPointsPy);
    centroidsSize  = PyList_Size(initialCentroidsPy);
    if(dataPointsSize == 0)
    {
        PyErr_SetString(PyExc_ValueError,"dataPointSizeError");
        return NULL;
    }
    if(centroidsSize == 0)
    {
        PyErr_SetString(PyExc_ValueError,"centroidsSizeError");
        return NULL;
    }


    initialCentroids =  spkmeansmodule_getInitialCentroids(initialCentroidsPy);
    dataPoints = spkmeansmodule_getDataPoints_old(dataPointsPy);

   
    if(dataPoints == NULL)
    {
        centroidsDestroy(initialCentroids,centroidsSize);
        PyErr_SetString(PyExc_ValueError,"dataPointCastingError");
        return NULL;
    }

    if(initialCentroids == NULL)
    {
        clusterPointDestroy(dataPoints,dataPointsSize);
        PyErr_SetString(PyExc_ValueError,"centroidsCastingError");
        return NULL;
    }
    
    updatedCentroids = fit(
        dataPoints,dataPointsSize,initialCentroids,centroidsSize,iter,eps
    );
    
    if(updatedCentroids == NULL)
    {
        PyErr_SetString(PyExc_ValueError,"AlgorithmError");
        return NULL;
    }
    return spkmeansmodule_getList(updatedCentroids,centroidsSize,dim);
}
static PyObject*spkmeansmodule_getList(Centroid*updatedCentroids,int k,int dim)
{
    PyObject* py_list;
    PyObject* sub_list;
    py_list = PyList_New(k);
    int i;
    int j;
    for(i=0;i<k;++i)
    {
        sub_list = PyList_New(dim);
        PyList_SetItem(py_list,i,sub_list);
        for(j=0;j<dim;j++)
        {
            PyList_SetItem(sub_list,j,PyFloat_FromDouble(updatedCentroids[i].coords[j]));
        }
    }
    return py_list;
}

static int spkmeansmodule_isListOfLists_old(PyObject*dataPointsPy)
{
    /*
        Check that argument is a list of lists
    */
    if(!PyList_Check(dataPointsPy))
    {
        PyErr_SetString(PyExc_ValueError,"MustSupplyPythonList0");
        return 0;
    }
    if(!PyList_Check(PyList_GetItem(dataPointsPy,0)))
    {
        PyErr_SetString(PyExc_ValueError,"MustSupplyPythonListOfLists1");
        return 0;
    }
    return 1;
}

static ClusterPoint * spkmeansmodule_getDataPoints_old(PyObject*dataPointsPy)
{

    ClusterPoint *dataPoints;
    Py_ssize_t n;
    Py_ssize_t dim;
    Py_ssize_t i,j;

    if(spkmeansmodule_isListOfLists_old(dataPointsPy) == 0)
    {
        return NULL;
    }
    /*
        Allocate and get values
    */
    n = PyList_Size(dataPointsPy);
    dim = PyList_Size(PyList_GetItem(dataPointsPy,0));
    dataPoints = (ClusterPoint*) malloc(sizeof(ClusterPoint) * n);
    if(dataPoints == NULL)
    {
        PyErr_SetString(PyExc_ValueError,"MallocF0");
        return NULL;
    }
    /*
        Fill ClusterPoint
    */
    for(i=0;i<n;++i)
    {
        dataPoints[i].point.dimention = dim;
        dataPoints[i].point.coords = (double*)malloc(sizeof(double) * dim);
        if(dataPoints[i].point.coords == NULL)
        {
            clusterPointDestroy(dataPoints,i);
            PyErr_SetString(PyExc_ValueError,"MallocF1");
            return NULL;
        }
        for(j=0; j<dim ;++j)
        {
            (dataPoints[i]).point.coords[j] = 
                    PyFloat_AsDouble(
                        PyList_GetItem(PyList_GetItem(dataPointsPy,i),j)
                    );
        }        
    }
    return dataPoints;
}
static Centroid * spkmeansmodule_getInitialCentroids(PyObject*initialCentroidsPy)
{
    Centroid *initialCentroids;
    Py_ssize_t k;
    Py_ssize_t dim;
    Py_ssize_t i,j;
    if(spkmeansmodule_isListOfLists_old(initialCentroidsPy) == 0)
    {
        return NULL;
    }
    k = PyList_Size(initialCentroidsPy);
    dim = PyList_Size(PyList_GetItem(initialCentroidsPy,0));
    initialCentroids = (Centroid*) malloc(sizeof(Centroid) * k);
    if(initialCentroids == NULL)
    {
        PyErr_SetString(PyExc_ValueError,"MallocF0");
        return NULL;
    }
    /*
        Fill Centroids
    */
    for(i=0;i<k;++i)
    {
        initialCentroids[i].dimention = dim;
        initialCentroids[i].coords = (double*)malloc(sizeof(double) * dim);
        if(initialCentroids[i].coords == NULL)
        {
            centroidsDestroy(initialCentroids,i);
            PyErr_SetString(PyExc_ValueError,"MallocF1");
            return NULL;
        }
        
        for(j=0; j<dim ;++j)
        {
            initialCentroids[i].coords[j] = 
                    PyFloat_AsDouble(
                        PyList_GetItem(PyList_GetItem(initialCentroidsPy,i),j)
                    );
        }        
    }
    return initialCentroids;

}

// module's function table
static PyMethodDef Spkmeans_FunctionsTable[] = {
    {"wam",
     spkmeansmodule_wam,
     METH_VARARGS,
     "passing the datapoints, and get weighted adj matrix "},
    {"ddg",
     spkmeansmodule_ddg,
     METH_VARARGS,
     "passing the datapoints, and get diagonal degree matrix "},
    {"gl",
     spkmeansmodule_gl,
     METH_VARARGS,
     "passing the datapoints, and get graph Laplacian matrix."},
    {"jacobi",
     spkmeansmodule_jacobi,
     METH_VARARGS,
     "passing the matrix, and get the eigenvalues and eigenvectors"},
    {"spk",
     spkmeansmodule_spk,
     METH_VARARGS,
     "passing the matrix, and get the eigenvalues and eigenvectors"},
    {NULL, NULL, 0, NULL}
    };
// modules definition
static struct PyModuleDef Spkmeans_Module = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",                                              // name of module exposed to Python
    "Spkmeans Python wrapper for custom C extension library.", // module documentation
    -1,
    Spkmeans_FunctionsTable};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    return PyModule_Create(&Spkmeans_Module);
}