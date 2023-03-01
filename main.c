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
    linked_list file_data;
    context c = {0,0,0};
    char *goal,*file_name;
    unsigned int status;
    FILE* file;
    vector* data_vecs;

    /*
        Validate Arguments
    */
    if (argc != 3) /*There is no need for input validation in this assignment Arel, don't waste too much time on it.*/
	{
		ERROR_AND_EXIT();	
	}
    list_init(&file_data); /* Shouldn't we first alloc space for the file_data on the heap?*/
    goal = argv[1]; /*Increased both parameters by 1, I belive it should be this way.*/
    file_name = argv[2];


    /*
        Open File
    */
    file = fopen(file_name,"r");
    if(file == NULL)
    {
        ERROR_AND_EXIT();	
    }
   

    /* 
        Get all data in string form
    */
	if (ioparser_parse_file_linked_list(&c,&file_data, file))
	{
        list_destroy(&file_data);
        fclose(file);
        ERROR_AND_EXIT();	
	}
    fclose(file);
    data_vecs = (vector *)malloc(sizeof(vector) * c.datapoint_count);
    if (data_vecs == NULL )
    {
        list_destroy(&file_data);
        ERROR_AND_EXIT();
    }
    
    list_reverse(&file_data); /*Not sure I understand why line reversal is necessary, Think it probably reverses data_vecs order too.*/
    status = ioparser_parse_data_points(&c,&file_data,data_vecs); 
    list_destroy(&file_data);
    if(status != c.datapoint_count)
    {
        vector_destroy(data_vecs,status);
        ERROR_AND_EXIT();
    }
    if(strcmp(goal,JACOBI) == 0)
    {
        jacobi->matrix = data_vecs;
        jacobi->col = c.datapoint_count;
        jacobi->row = c.datapoint_count;
        /* Add Jacobi call Temp error*/
        ERROR_AND_EXIT();	

    }
    else
    {
        /* Non Jacobi */
        wam = spkmeans_wam(&c, data_vecs);
        vector_destroy(data_vecs,c.datapoint_count);
        data_vecs = NULL;
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
    vector_destroy(data_vecs,c.datapoint_count);	
    matrix_destroy(wam);
    matrix_destroy(ddg);
    matrix_destroy(gl);
    matrix_destroy(jacobi);
    ERROR_AND_EXIT();
}