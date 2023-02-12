#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "spkmeans.h"

// module's function table
static PyMethodDef Spkmeans_FunctionsTable[] = {
	{
		"spk",																									   // name exposed to Python
		(PyCFunction)Spkmeansmodule_spk,																		   // C wrapper function
		METH_VARARGS,																							   // received variable args (but really just 1)
		"passing the initial centroids, the datapoints and other arguments if needed, and get the final centroids" // documentation
	},
	{"wam",							  // name exposed to Python
	 (PyCFunction)Spkmeansmodule_wam, // C wrapper function
	 METH_VARARGS,					  // received variable args (but really just 1)
	 "passing the datapoints, and get weighted
	 adj matrix " // documentation
	},
	{"ddg",							  // name exposed to Python
	 (PyCFunction)Spkmeansmodule_ddg, // C wrapper function
	 METH_VARARGS,					  // received variable args (but really just 1)
	 "passing the datapoints, and get diagonal
	 degree matrix " // documentation
	},
	{"gl",							 // name exposed to Python
	 (PyCFunction)Spkmeansmodule_gl, // C wrapper function
	 METH_VARARGS,					 // received variable args (but really just 1)
	 "passing the datapoints, and get graph
	 Laplacian matrix." // documentation
	},
	{
		"jacobi",																								   // name exposed to Python
		(PyCFunction)Spkmeansmodule_jacobi,																		   // C wrapper function
		METH_VARARGS,																							   // received variable args (but really just 1)
		"passing the initial centroids, the datapoints and other arguments if needed, and get the final centroids" // documentation
	},
	{NULL, NULL, 0, NULL}};

// modules definition
static struct PyModuleDef Spkmeans_Module = {
	PyModuleDef_HEAD_INIT,
	"spkmeans",												   // name of module exposed to Python
	"Spkmeans Python wrapper for custom C extension library.", // module documentation
	-1,
	Spkmeans_FunctionsTable};

PyMODINIT_FUNC PyInit_kmeans(void)
{
	return PyModule_Create(&Spkmeans_Module);
}