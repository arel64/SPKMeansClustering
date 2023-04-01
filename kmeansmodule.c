#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeanslib.h"

static ClusterPoint * getDataPoints(PyObject*dataPointsPy);
static Centroid * getinitialCentroids(PyObject*initialCentroidsPy);
static int isListOfLists(PyObject*dataPointsPy);
static PyObject*GetList(Centroid*updatedCentroids,int k,int dim);
static PyObject *Kmeansmodule_fit(PyObject *self, PyObject *args) {
    

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


    initialCentroids =  getinitialCentroids(initialCentroidsPy);
    dataPoints = getDataPoints(dataPointsPy);

   
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
    return GetList(updatedCentroids,centroidsSize,dim);
}
static PyObject*GetList(Centroid*updatedCentroids,int k,int dim)
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

static int isListOfLists(PyObject*dataPointsPy)
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

static ClusterPoint * getDataPoints(PyObject*dataPointsPy)
{

    ClusterPoint *dataPoints;
    Py_ssize_t n;
    Py_ssize_t dim;
    Py_ssize_t i,j;

    if(isListOfLists(dataPointsPy) == 0)
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
static Centroid * getinitialCentroids(PyObject*initialCentroidsPy)
{
    Centroid *initialCentroids;
    Py_ssize_t k;
    Py_ssize_t dim;
    Py_ssize_t i,j;
    if(isListOfLists(initialCentroidsPy) == 0)
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
static PyMethodDef KmeansLib_FunctionsTable[] = {
    {
        "spk", // name exposed to Python
        (PyCFunction)Kmeansmodule_fit, // C wrapper function
        METH_VARARGS, // received variable args (but really just 1)
        "Returns Final Centriods" // documentation
    }, {
        NULL, NULL, 0, NULL
    }
};

// modules definition
static struct PyModuleDef KmeansLib_Module = {
    PyModuleDef_HEAD_INIT,
    "kmeans",     // name of module exposed to Python
    "Kmeans Python wrapper for custom C extension library.", // module documentation
    -1,
    KmeansLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_kmeans(void) {
    return PyModule_Create(&KmeansLib_Module);
}