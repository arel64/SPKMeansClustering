#ifndef SPKMEANS
#define SPKMEANS
#include "matrix.h"
#include "vector.h"

matrix* spkmeans_wam(const  context*const,const point* const data_points);
matrix* spkmeans_ddg(const matrix* const m);
matrix* spkmeans_gl(const matrix* const wam,const matrix* const ddg);
#endif
