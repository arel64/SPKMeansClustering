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
void spkmeans_jacobi(matrix* m, matrix* ret[]){
    int max_off_diag_loc[2];
    bool convergence = false;
    double theta, t, c, s;
    int i, j;
    matrix* ret_matrix = matrix_create_identity_matrix(m->row);
    matrix* m_; /*Will represent A' in the assignment's descrtiption.*/
    matrix* curr_rotation;
    matrix* temp_matrix; /*A temporary matrix for data loss prevention.*/
    vector first_vector = (vector) malloc(sizeof(double) * m->col);
    vector second_vector = (vector) malloc(sizeof(double) * m->col);


    for(i = 0;!convergence && i < SPKMEANS_MAX_ROATATIONS; i++)
    {
        matrix_find_max_off_diag(m, max_off_diag_loc);
        theta = SPKMEANS_THETA_FORMULA(m, max_off_diag_loc);
        t = SPKMEANS_T_FORMULA(theta);
        c = SPKMEANS_C_FORMULA(t);
        s = c * t;
        /*
        This whole section does WAY to many allocations, there is no need to matrix_create every iteration.
        TODO:: FIX MULTIPLE ALLOCS
        */
        curr_rotation = matrix_create_identity_matrix(m->col);
        curr_rotation->matrix[max_off_diag_loc[0]][max_off_diag_loc[0]] = c;
        curr_rotation->matrix[max_off_diag_loc[1]][max_off_diag_loc[1]] = c;
        curr_rotation->matrix[max_off_diag_loc[0]][max_off_diag_loc[1]] = s;
        curr_rotation->matrix[max_off_diag_loc[1]][max_off_diag_loc[0]] = -s;
        temp_matrix = matrix_multiply(ret_matrix, curr_rotation);
        matrix_destroy(ret_matrix);
        matrix_destroy(curr_rotation);
        ret_matrix = temp_matrix;
        m_ = matrix_create(m->row, m->col);
        matrix_copyinto_matrix(m_, m);
        for(j = 0; j < 2; j++)
        {
            vector_copyinto_vector(&first_vector, &m->matrix[max_off_diag_loc[j]], m->col);
            vector_copyinto_vector(&second_vector, &m->matrix[max_off_diag_loc[1-j]], m->col);
            vector_multipy_vector_by_scalar(&first_vector, c, m->col);
            vector_multipy_vector_by_scalar(&second_vector, (pow(-1,1-j))*s, m->col);
            vector_each_cell(&m_->matrix[max_off_diag_loc[j]],&first_vector,&second_vector,0,ADD_VECTOR,m->col);
            matrix_tranpose_row(m_, max_off_diag_loc[j]);
            m_->matrix[max_off_diag_loc[j]][max_off_diag_loc[j]] = pow(c,2) * m->matrix[max_off_diag_loc[j]][max_off_diag_loc[j]] + pow(s,2) * m->matrix[max_off_diag_loc[1 - j]][max_off_diag_loc[1 - j]] + pow(-1, 1 + j) * 2 * s * c * m->matrix[max_off_diag_loc[j]][max_off_diag_loc[1 - j]];
        }
        for(j = 0; j < 2; j++)
        {
            m_->matrix[max_off_diag_loc[j]][max_off_diag_loc[1-j]] = 0;
        }
        convergence = matrix_check_convergence(m, m_);
        matrix_destroy(m);
        m = m_;
    }
    vector_destroy(&first_vector, 1);
    vector_destroy(&second_vector, 1);
    ret[0] = matrix_create(1, m->row);
    matrix_copy_diag_into_row_matrix(ret[0], m);
    matrix_destroy(m);
    ret[1] = ret_matrix;
}
