#include "matrix.h"
void    matrix_each_cell(matrix* into,const matrix *const left,const matrix *const right, const double scalar, int* const loc, double* const ret_scalr, unsigned how);
double  matrix_sum_axis(const matrix*const m,const unsigned vector_num,const unsigned axis);


matrix* matrix_create(const unsigned row,const unsigned col)
{
    unsigned i = 0;
    double* alloc = NULL;
    matrix* ret = (matrix*) malloc(sizeof(matrix));
    if(row==0 || col==0 || ret ==NULL)
    {
        return NULL;
    }
    ret->col = col;
    ret->row = row;

    alloc = (double*)malloc(row*col* sizeof(double));
    ret->matrix = (double**)calloc(row, sizeof(double *));
    for(;i < row; i++) 
        ret->matrix[i] = alloc + col*i;
    return  ret;
}
matrix* matrix_create_from_data(const unsigned row,const unsigned col,double** data)
{
    matrix* m = matrix_create(row,col);
    size_t i = 0,j = 0;
    for(;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            m->matrix[i][j] = data[i][j];
        }
    }
    return m;
}
matrix* matrix_add(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,right->col);
    matrix_each_cell(ret,left,right,0,NULL,NULL,ADD_MATRIX);
    return ret;
}
matrix* matrix_subtract(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,left->col);
    matrix_each_cell(ret,left,right,0,NULL,NULL,SUBTRACT_MATRIX);
    return ret;
}
matrix* matrix_multiply(const matrix *const left, const matrix*const right)
{
    matrix* ret = matrix_create(left->row,right->col);
    unsigned i = 0 , j = 0 , k = 0;
    matrix_zerofill(ret);
    if(ret == NULL) return NULL;

    for(;i<ret->row;i++)
    {
        for(j=0;j<ret->col;j++)
        {
            for (k = 0; k < right->row; k++) {
                ret->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return ret;
}


matrix* matrix_multiply_scalar(const matrix *const m,double scalar)
{
    matrix* ret = matrix_create(m->row,m->col);
    matrix_each_cell(ret,m,NULL,scalar,NULL,NULL,MULTIPLY_SCALAR_MATRIX);
    return ret;
}
void matrix_zerofill(matrix * m)
{
    matrix_each_cell(m, NULL, NULL, 0, NULL, NULL, ZEROFILL_MATRIX);
}
void matrix_each_cell(matrix* into,const matrix *const left,const matrix *const right,const double scalar, int* const ret_loc, double* ret_scalar, unsigned how)
{
    size_t i = 0 , j = 0;
    double dub = -1;

    if(into == NULL) return;

    for(;i<into->row;i++)
    {
        for(j=0;j<into->col;j++)
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
                    into->matrix[i][j] = left->matrix[i][j] * scalar;
                    break;
                case COPYINTO_MATRIX:
                    into->matrix[i][j] = left->matrix[i][j];
                    break;
                case ZEROFILL_MATRIX:
                    into->matrix[i][j] = 0;
                    break;
                case PRINT_MATRIX:
                    printf("%.4f", into->matrix[i][j]);
                    if (j != into->col - 1)
                    {
                        printf(",");
                    }
                    else
                    {
                        printf("\n");
                    }
                    break;
                case FIND_MAX_OFF_DIAGNAL:
                    if(i >= j)
                    {
                        j = i;
                    }
                    else if(fabs(into->matrix[i][j]) > dub)
                    {
                        dub = fabs(into->matrix[i][j]);
                        *ret_loc = i;
                        *(ret_loc + 1) = j;
                    }
                    break;
                case CALCULATE_OFF:
                    if(j < i)
                    {
                        j = i;
                    }
                    else if(i < j)
                    {
                        *(ret_scalar) += 2 * pow(into->matrix[i][j], 2);
                    }
                    break;
                case TRANSPOSE:
                    if(j < i)
                    {
                        j = i;
                    }
                    else if(i < j)
                    {
                        dub = into->matrix[i][j];
                        into->matrix[i][j] = into->matrix[j][i];
                        into->matrix[j][i] = dub;
                    }
                    break;
                default:
                    return;
            }
        }
    }
}
int matrix_is_equal(const matrix*const m1,const matrix*const m2,double max_delta)
{
    size_t i=0,j=0;
    if(m1->row != m2->row || m1->col != m2->col)
    {
        return 0;
    }
    for(;i<m1->row;i++)
    {
        for(j=0;j<m1->col;j++)
        {
            if(fabs(m1->matrix[i][j] - m2->matrix[i][j]) > fabs(max_delta))
            {
                return 0;
            }
        }
    }
    return 1;
}
void    matrix_copyinto_matrix(matrix *copyInto, const matrix*const copyFrom)
{
    matrix_each_cell(copyInto,copyFrom,NULL,0, NULL, NULL, COPYINTO_MATRIX);
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
    double end =(axis==0)?m->col:m->row;
    for(;i<end;i++)
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
void matrix_print(matrix* m)
{
    matrix_each_cell(m, NULL, NULL, 0, NULL, NULL, PRINT_MATRIX);
}
/*
    @pre: m is symetric.
*/
void matrix_find_max_off_diag(matrix* const m, int* const loc){
    matrix_each_cell(m, NULL, NULL, 0, loc, NULL, FIND_MAX_OFF_DIAGNAL);
}
matrix* matrix_create_identity_matrix(const unsigned size){
    size_t i = 0;
    matrix* ret = matrix_create(size, size);
    matrix_zerofill(ret);
    for(; i < size; i++)
    {
        ret->matrix[i][i] = 1;
    }
    return ret;
}
/*
    @pre: m is quadratic.
*/
void matrix_tranpose_row(matrix* m, const unsigned row)
{
    size_t i = 0;
    for(; i < m->col; i++)
    {
        m->matrix[i][row] = m->matrix[row][i];
    }
}
/*
    @pre: m is symmetric.
*/
double matrix_calc_off(matrix* const m)
{
    double ret = 0;
    matrix_each_cell(m,NULL,NULL,0,NULL,&ret,CALCULATE_OFF);
    return ret;
}
bool matrix_check_convergence(matrix* const m, matrix* const m_)
{
    return (matrix_calc_off(m) - matrix_calc_off(m_)) <= EPSILON;
}
void matrix_transpose(matrix* mat){
    matrix_each_cell(mat, NULL, NULL, 0, NULL, NULL, TRANSPOSE);
}
/*
    @pre: from is quandratic.
    @pre: into is a row matrix with the same length as the size of from.
*/
void matrix_copy_diag_into_row_matrix(matrix* const into, const matrix* const from)
{
    size_t i = 0;
    for(;i < into -> col; i++)
    {
        into->matrix[0][i] = from->matrix[i][i];
    }
}
void matrix_destroy(matrix* m)
{
    free(m->matrix[0]);
    free(m->matrix);
    free(m);
}
