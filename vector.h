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

int     vector_init_arr                  (const context*const ,vector *);
double  vector_euclidean_distance        (const context*const ,const vector *, const vector*);
void    vector_sum_into_vector           (const context*const ,vector *sumInto, const vector*const sumFrom);
void    vector_multipy_vector_by_scalar  (const context*const ,const vector *, double);
void    vector_copyinto_vector           (const context*const ,vector *copyInto, const vector*const copyFrom);
void    vector_each_cell                 (const context*const ,vector* into,const vector *const left,const vector *const right,const double scalar,unsigned how);
void    vec_destroy_c                    (const context*const ,vector *);
void    vec_destroy                      (                     vector *,unsigned);

#endif