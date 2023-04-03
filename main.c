#include "matrix.h"
#include "spkmeans.h"
#include "ioparser.h"
#include "cleanup.h"

#define WAM "wam"
#define DDG "ddg"
#define GL "gl"
#define JACOBI "jacobi"


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
    
    if(strcmp(goal,JACOBI) == 0)
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
        if(strcmp(goal,WAM) == 0)
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
        if(strcmp(goal,DDG) == 0)
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
        if(strcmp(goal,GL) == 0)
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