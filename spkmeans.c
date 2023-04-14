
#include "spkmeans.h"



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
    free(first_vector);
    free(second_vector);
    matrix_destroy(a);
    matrix_destroy(a_next);
    matrix_destroy(temp);
    matrix_destroy(curr_rotation);
    ret[1] = ret_matrix;
}


int main(int argc, char **argv)
{
    matrix* wam = NULL,*ddg =NULL,*gl = NULL,*jacobi =NULL;
    context c = {0,0,0};
    char *goal,*file_name;
    vector* data_vecs;


    /*
        Validate Arguments
    */
    if (argc != 3)
	{
		ERROR_AND_EXIT();	
	}
   
    goal = argv[1];
    file_name = argv[2];
    data_vecs = ioparser_parse_file_to_data_points(&c, file_name);

    if(data_vecs == NULL)
    {
        ERROR_AND_EXIT();
    }
    
    if(strcmp(goal,JACOBI_TXT) == 0)
    {
        size_t i = 0;
        size_t j = 0;
        matrix* jacobi_returned_matrices[2];
        /*      
            The returned matrices, first component is a row vector of the eigenvalues, 
            second is the matrix of which vectors are the corresponding eigenvectors.
        */
        jacobi = matrix_create(c.datapoint_count,c.datapoint_count);
        for (i = 0; i < c.datapoint_count; i++)
        {
            for(j=0; j < c.datapoint_count; j++)
                jacobi->matrix[i][j] = data_vecs[i][j];
        }
        vector_destroy(&data_vecs,c.datapoint_count);
        spkmeans_jacobi(jacobi, jacobi_returned_matrices);
        matrix_print(jacobi_returned_matrices[0]);
        matrix_print(jacobi_returned_matrices[1]);
        matrix_destroy(jacobi_returned_matrices[0]);
        matrix_destroy(jacobi_returned_matrices[1]);
        return 0;
    }
    else
    {
        /* Non Jacobi */
        wam = spkmeans_wam(&c, data_vecs);
        vector_destroy(&data_vecs,c.datapoint_count);
        if(wam == NULL)
        {
            ERROR_AND_EXIT();
        }
        if(strcmp(goal,WAM_TXT) == 0)
        {
            matrix_print(wam);
            matrix_destroy(wam);
            return 0;
        }
        ddg = spkmeans_ddg(wam);
        if(ddg == NULL)
        {
            matrix_destroy(wam);
            ERROR_AND_EXIT();
        }
        if(strcmp(goal,DDG_TXT) == 0)
        {    
            matrix_print(ddg);
            matrix_destroy(wam);
            matrix_destroy(ddg);
            return 0;
        }
        gl = spkmeans_gl(wam,ddg);
        matrix_destroy(wam);
        matrix_destroy(ddg);
        wam = NULL;
        ddg = NULL;
        if(gl == NULL)
        {
            ERROR_AND_EXIT();
        }
        if(strcmp(goal,GL_TXT) == 0)
        {
            matrix_print(gl);
            matrix_destroy(gl);
            return 0;
        }
    }
    /*
     This part is unreachable but stays for the future
    */
    vector_destroy(&data_vecs,c.datapoint_count);	
    matrix_destroy(wam);
    matrix_destroy(ddg);
    matrix_destroy(gl);
    matrix_destroy(jacobi);
    ERROR_AND_EXIT();
}