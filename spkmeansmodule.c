#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "spkmeans.h"
static PyObject *Spkmeansmodule_spk(PyObject *self, PyObject *args)
{
	
	return NULL;
}
static PyObject *Spkmeansmodule_wam(PyObject *self, PyObject *args)
{
	return NULL;
}
static PyObject *Spkmeansmodule_ddg(PyObject *self, PyObject *args)
{
	return NULL;
}
static PyObject *Spkmeansmodule_gl(PyObject *self, PyObject *args)
{
	return NULL;
}
static PyObject *Spkmeansmodule_jacobi(PyObject *self, PyObject *args)
{
	return NULL;
}

// module's function table
static PyMethodDef Spkmeans_FunctionsTable[] = {
	{
		"spk",																									   
		(PyCFunction)Spkmeansmodule_spk,																		   
		METH_VARARGS,																							   
		"passing the initial centroids, the datapoints and other arguments if needed, and get the final centroids" 
	},
	{
		"wam",							  
		(PyCFunction)Spkmeansmodule_wam, 
		METH_VARARGS,					  
		"passing the datapoints, and get weighted adj matrix " 
	},
	{
		"ddg",							  
		(PyCFunction)Spkmeansmodule_ddg, 
		METH_VARARGS,					  
		"passing the datapoints, and get diagonal degree matrix " 
	},
	{
		"gl",							 
		(PyCFunction)Spkmeansmodule_gl, 
		METH_VARARGS,					 
		"passing the datapoints, and get graph Laplacian matrix." 
	},
	{
		"jacobi",																								   
		(PyCFunction)Spkmeansmodule_jacobi,																		   
		METH_VARARGS,																							   
		"passing the matrix, and get the eigenvalues and eigenvectors" 
	},
	{NULL, NULL, 0, NULL}
	};
// modules definition
static struct PyModuleDef Spkmeans_Module = {
	PyModuleDef_HEAD_INIT,
	"mykmeanssp",												   // name of module exposed to Python
	"Spkmeans Python wrapper for custom C extension library.", // module documentation
	-1,
	Spkmeans_FunctionsTable};

PyMODINIT_FUNC PyInit_kmeans(void)
{
	return PyModule_Create(&Spkmeans_Module);
}