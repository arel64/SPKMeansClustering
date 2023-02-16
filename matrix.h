#ifndef MATRIX
#define MATRIX
#define ADD_MATRIX 101
#define SUBTRACT_MATRIX 201
#define MULTIPLY_SCALAR_MATRIX 301
#define COPYINTO_MATRIX 401
#define MULTIPLY_MATRIX 501

#include "context.h"
#include "vector.h"
typedef struct{
    double** matrix;
    unsigned row;
    unsigned col;
}matrix;


matrix* matrix_create(const unsigned row,const unsigned col);
matrix* matrix_add(const matrix *const left, const matrix*const right);
matrix* matrix_subtract(const matrix *const left, const matrix*const right);
matrix* matrix_multiply(const matrix *const left, const matrix*const right);
matrix* matrix_multiply_scalar(const matrix *const,double);
double  matrix_sum_row(const matrix*const m,const unsigned row);
double  matrix_sum_col(const matrix*const m,const unsigned col);
void    matrix_copyinto_matrix(matrix *copyInto, const matrix*const copyFrom);
void    matrix_destroy(matrix* m);
#endif