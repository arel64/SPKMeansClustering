#include "spkmeans.h"
#include "matrix.h"


matrix* spkmeans_wam(const  context*const c,const point* const data_points)
{
    size_t i=0,j=0;
    size_t n=c->datapoint_count;
    matrix* ret = matrix_create(n,n);
    if(ret==NULL)
    {
        return NULL;
    }
    for(;i<n;i++)
    {
        for(j = 0;j<n;j++)
        {
            if(i==j)
            {
                ret->matrix[i][j] = 0;
            }
            else if(i<j) 
            {
                ret->matrix[i][j] = SPKMEANS_WAM_FORMULA(data_points[i],data_points[j],c->dimention);
            }
            else
            {
                ret->matrix[i][j] =  ret->matrix[j][i];
            }
        }
    }
    return ret;
}

matrix* spkmeans_ddg(const matrix* const wam)
{
    size_t i=0;
    unsigned size = wam->row;
    matrix* ret = matrix_create(size,size);
    if(ret==NULL)
    {
        return NULL;
    }
    matrix_zerofill(ret);
    for(;i<size;i++)
    {
        ret->matrix[i][i] =  matrix_sum_row(wam,i);
        
    }
    return ret;
}
matrix* spkmeans_gl(const matrix* const wam,const matrix* const ddg)
{
    return matrix_subtract(ddg, wam);
}
/*
@pre: m is symetric.
*/
void spkmeans_jacobi(matrix* a, matrix* ret[]){
    int max_off_diag_loc[2];
    bool convergence = false;
    double theta, t, c, s;
    unsigned int i, j,p;
    matrix* ret_matrix = matrix_create_identity_matrix(a->row);
    matrix* a_next = matrix_create(a->row, a->col); /*Will represent A' in the assignment's descrtiption.*/
    matrix* temp = matrix_create(a->row, a->col);
    matrix* curr_rotation = matrix_create_identity_matrix(a->col);
    vector first_vector = (vector) malloc(sizeof(double) * a->col);
    vector second_vector = (vector) malloc(sizeof(double) * a->col);

    for(i = 0;!convergence && i < SPKMEANS_MAX_ROATATIONS; i++)
    {
        matrix_find_max_off_diag(a, max_off_diag_loc);
        if(a->matrix[max_off_diag_loc[0]][max_off_diag_loc[1]] == 0)
        {
            break;
        }
        theta = SPKMEANS_THETA_FORMULA(a, max_off_diag_loc);
        t = SPKMEANS_T_FORMULA(theta);
        c = SPKMEANS_C_FORMULA(t);
        s = c * t;
        matrix_copyinto_matrix(temp, ret_matrix);
        for(j = 0; j < 2; j++)
        {
            vector_copyinto_vector(&first_vector, &temp->matrix[max_off_diag_loc[j]], a->col);
            vector_copyinto_vector(&second_vector, &temp->matrix[max_off_diag_loc[1-j]], a->col);
            vector_multipy_vector_by_scalar(&first_vector, c, a->col);
            vector_multipy_vector_by_scalar(&second_vector, pow(-1,1-j) *s, a->col);
            vector_each_cell(&ret_matrix->matrix[max_off_diag_loc[j]],&first_vector,&second_vector,0,ADD_VECTOR,a->col);
        }
        for(p = 0; p < 2; p++)
        {
            for(j = 0; j < 2; j++)
            {
                if(p == j)
                    curr_rotation->matrix[max_off_diag_loc[p]][max_off_diag_loc[j]] = 1;
                else
                    curr_rotation->matrix[max_off_diag_loc[p]][max_off_diag_loc[j]] = 0;       
            }
        }
        matrix_copyinto_matrix(a_next, a);
        for(j = 0; j < 2; j++)
        {
            vector_copyinto_vector(&first_vector, &a->matrix[max_off_diag_loc[j]], a->col);
            vector_copyinto_vector(&second_vector, &a->matrix[max_off_diag_loc[1-j]], a->col);
            vector_multipy_vector_by_scalar(&first_vector, c, a->col);
            vector_multipy_vector_by_scalar(&second_vector, (pow(-1,1-j)) * s, a->col);
            vector_each_cell(&a_next->matrix[max_off_diag_loc[j]],&first_vector,&second_vector,0,ADD_VECTOR,a->col);
            matrix_tranpose_row(a_next, max_off_diag_loc[j]);
            a_next->matrix[max_off_diag_loc[j]][max_off_diag_loc[j]] = pow(c,2) * a->matrix[max_off_diag_loc[j]][max_off_diag_loc[j]] + pow(s,2) * a->matrix[max_off_diag_loc[1 - j]][max_off_diag_loc[1 - j]] + pow(-1, 1 + j) * 2 * s * c * a->matrix[max_off_diag_loc[j]][max_off_diag_loc[1 - j]];
        }
        for(j = 0; j < 2; j++)
        {
            a_next->matrix[max_off_diag_loc[j]][max_off_diag_loc[1-j]] = 0;
        }
        convergence = matrix_check_convergence(a, a_next);
        matrix_copyinto_matrix(a, a_next);
    }
    ret[0] = matrix_create(1, a->row);
    matrix_copy_diag_into_row_matrix(ret[0], a);
    for (p = 0; p < ret[0]->col; p++)
    {
        if (ZERO_DIFFERENTIATOR(ret[0]->matrix[0][p]))
        {
            vector_multipy_vector_by_scalar(&ret_matrix->matrix[p], -1, a->col);
        }
    }
    matrix_transpose(ret_matrix);
    vector_destroy(&first_vector, 1);
    vector_destroy(&second_vector, 1);
    matrix_destroy(a);
    matrix_destroy(a_next);
    matrix_destroy(temp);
    ret[1] = ret_matrix;
}
