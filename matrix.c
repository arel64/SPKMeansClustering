#include "matrix.h"
void    matrix_each_cell(matrix* into,const matrix *const left,const matrix *const right,const double scalar,unsigned how);
double  matrix_sum_axis(const matrix*const m,const unsigned vector_num,const unsigned axis);


matrix* matrix_create(const unsigned row,const unsigned col)
{
    matrix* ret;
    double* alloc;
    alloc = (double*)malloc(sizeof(double)*row*col);
    if(alloc == NULL)
    {
        return NULL;
    }
    ret = (matrix*)malloc(sizeof(matrix));
    if(ret == NULL)
    {
        return NULL;
    }
    ret->col = col;
    ret->row = row;
    ret->matrix = &alloc;
   
    return ret;
}
matrix* matrix_add(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,right->col);
    matrix_each_cell(ret,left,right,0,ADD_MATRIX);
    return ret;
}
matrix* matrix_subtract(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,left->col);
    matrix_each_cell(ret,left,right,0,SUBTRACT_MATRIX);
    return ret;
}
matrix* matrix_multiply(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,right->col);
    matrix_each_cell(ret,left,right,0,MULTIPLY_MATRIX);
    return ret;
}
matrix* matrix_multiply_scalar(const matrix *const m,double scalar)
{
    matrix* ret = matrix_create(m->row,m->col);
    matrix_each_cell(ret,m,NULL,scalar,MULTIPLY_SCALAR);
    return ret;
}
void matrix_each_cell(matrix* into,const matrix *const left,const matrix *const right,const double scalar,unsigned how)
{
    size_t i = 0 , j = 0;
    if(into == NULL) return;

    for(;i<into->row;i++)
    {
        for(;j<into->row;j++)
        {
            switch (how)
            {
            case ADD_MATRIX:
                into->matrix[i][j] = left->matrix[i][j] + right->matrix[i][j];
                break;
            case SUBTRACT_MATRIX:
                into->matrix[i][j] = left->matrix[i][j] - right->matrix[i][j];
                break;
            case MULTIPLY_SCALAR_MATRIX:
                into->matrix[i][j] = into->matrix[i][j] * scalar;
                break;
            case COPYINTO_MATRIX:
                into->matrix[i][j] = left->matrix[i][j];
                break;
            case MULTIPLY_MATRIX:
                into->matrix[i][j] = matrix_sum_row(left,i) * matrix_sum_col(right,j);
                break;
            default:
                return;
                break;
            }
        }
    }
}
void matrix_copyinto_matrix(matrix *copyInto, const matrix*const copyFrom)
{
    matrix_each_cell(copyInto,copyFrom,NULL,0,COPYINTO_MATRIX);
}
double  matrix_sum_row(const matrix*const m,const unsigned row)
{
    return matrix_sum_axis(m,row,0);
}
double  matrix_sum_col(const matrix*const m,const unsigned col)
{
    return matrix_sum_axis(m,col,1);
}
double  matrix_sum_axis(const matrix*const m,const unsigned vector_num,const unsigned axis)
{
    size_t i=0;
    double count = 0;
    for(;i<axis==0?m->col:m->row;i++)
    {
        if(axis==0)
        {
            count += m->matrix[vector_num][i];
        }
        else
        {
            count += m->matrix[i][vector_num];
        }
        
    }
    return count;
}
void    matrix_destroy(matrix* m)
{
    free(m);
}
