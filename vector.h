#ifndef VECTOR
#define VECTOR


#define ADD_VECTOR 100
#define MULTIPLY_SCALAR 200
#define COPYINTO_VECTOR 300

#include <stdlib.h>
#include <math.h>
#include "context.h"
typedef double* vector;
typedef vector point;
typedef point centroid;

int     vector_init_arr                 (vector *,const unsigned k,const unsigned dim);
double  vector_euclidean_distance       (const vector , const vector, const unsigned);
void    vector_sum_into_vector          (vector *sumInto, const vector*const sumFrom, const unsigned);
void    vector_multipy_vector_by_scalar (vector *, double, const unsigned);
void    vector_copyinto_vector          (vector *copyInto, const vector*const copyFrom, const unsigned);
double  vector_multiply                 (vector* vector_row,vector* vector_col, const unsigned);
void    vector_each_cell                (vector* into,const vector *const left,const vector *const right,const double scalar,unsigned how, const unsigned);
void    vector_destroy                  (vector **,unsigned);

#endif
