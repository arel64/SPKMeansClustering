#include "matrix.h"
void    matrix_each_cell(const context*const ,matrix* into,const matrix *const left,const matrix *const right,const double scalar,unsigned how);
double  matrix_sum_axis(const context*const c,const matrix*const m,const unsigned vector_num,const unsigned axis);


matrix* matrix_create(const context*const c)
{
    matrix* ret;
    double* alloc;
    size_t i = 0;
    unsigned n = c->datapoint_size;

    alloc = (double*)malloc(sizeof(double)*n*n);
    if(alloc == NULL)
    {
        return NULL;
    }
    ret = (matrix*)malloc(sizeof(matrix));
    if(ret == NULL)
    {
        return NULL;
    }
    *ret = &alloc;
   
    return ret;
}
matrix* matrix_add(const context*const c,const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(c);
    matrix_each_cell(c,ret,left,right,0,ADD_MATRIX);
    return ret;
}
matrix* matrix_subtract(const context*const c,const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(c);
    matrix_each_cell(c,ret,left,right,0,SUBTRACT_MATRIX);
    return ret;
}
matrix* matrix_multiply(const context*const c,const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(c);
    matrix_each_cell(c,ret,left,right,0,MULTIPLY_MATRIX);
    return ret;
}
matrix* matrix_multiply_scalar(const context*const c,const matrix *const,double)
{
    matrix* ret = matrix_create(c);
    matrix_each_cell(c,ret,NULL,NULL,0,MULTIPLY_SCALAR);
    return ret;
}
void matrix_each_cell(const context*const c,matrix* into,const matrix *const left,const matrix *const right,const double scalar,unsigned how)
{
    size_t i = 0 , j = 0;
    size_t n = c->datapoint_count;
    
    if(into == NULL) return;

    for(;i<n;i++)
    {
        for(;j<n;j++)
        {
            switch (how)
            {
            case ADD_MATRIX:
                (*into)[i][j] = (*left)[i][j] + (*right)[i][j];
                break;
            case SUBTRACT_MATRIX:
                (*into)[i][j] = (*left)[i][j] - (*right)[i][j];
                break;
            case MULTIPLY_SCALAR_MATRIX:
                (*into)[i][j] = (*into)[i][j] * scalar;
                break;
            case COPYINTO_MATRIX:
                (*into)[i][j] = (*left)[i][j];
                break;
            case MULTIPLY_MATRIX:
                (*into)[i][j] = matrix_sum_row(c,left,i) * matrix_sum_col(c,right,j);
                break;
            default:
                return;
                break;
            }
        }
    }
}
void    matrix_copyinto_matrix(const context*const c,matrix *copyInto, const matrix*const copyFrom)
{
    matrix_each_cell(c,copyInto,copyFrom,NULL,0,COPYINTO_MATRIX);
}
double  matrix_sum_row(const context*const c,const matrix*const m,const unsigned row)
{
    return matrix_sum_axis(c,m,row,0);
}
double  matrix_sum_col(const context*const c,const matrix*const m,const unsigned col)
{
    return matrix_sum_axis(c,m,col,1);
}
double  matrix_sum_axis(const context*const c,const matrix*const m,const unsigned vector_num,const unsigned axis)
{
    size_t i=0;
    double count = 0;
    for(;i<c->datapoint_size;i++)
    {
        if(axis==0)
        {
            count += (*m)[vector_num][i];
        }
        else
        {
            count += (*m)[i][vector_num];
        }
        
    }
    return count;
}
void    matrix_destroy(matrix* m)
{
    free(m);
}
