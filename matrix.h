#ifndef MATRIX
#define MATRIX
#define ADD_MATRIX 101
#define SUBTRACT_MATRIX 201
#define MULTIPLY_SCALAR_MATRIX 301
#define COPYINTO_MATRIX 401
#define MULTIPLY_MATRIX 501

#include "context.h"
#include "vector.h"
typedef double** matrix;


matrix* matrix_create(const context*const);
matrix* matrix_add(const context*const,const matrix *const left, const matrix*const right);
matrix* matrix_subtract(const context*const,const matrix *const left, const matrix*const right);
matrix* matrix_multiply(const context*const,const matrix *const left, const matrix*const right);
matrix* matrix_multiply_scalar(const context*const,const matrix *const,double);
double  matrix_sum_row(const context*const c,const matrix*const m,const unsigned row);
double  matrix_sum_col(const context*const c,const matrix*const m,const unsigned col);
void    matrix_copyinto_matrix(const context*const ,matrix *copyInto, const matrix*const copyFrom);
void    matrix_destroy(matrix* m);
#endif