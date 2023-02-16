#include "ioparser.h"
#include "cleanup.h"

#define WAM "wam"
#define DDG "ddg"
#define GL "gl"
#define JACOBI "jacobi"


cluster_point *data_points = NULL;
linked_list file_data;

int main(int argc, char **argv)
{
    context c = {0,0,0};
    char *goal,*file_name;
    unsigned int status;
    FILE* file;
    cluster_point* data_points;

    /*
        Validate Arguments
    */
    if (argc != 3)
	{
		ERROR_AND_EXIT();	
	}
    list_init(&file_data);
    goal = argv[0];
    file_name = argv[1];


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
        ERROR_AND_EXIT();	
	}
    if(strcmp(goal,JACOBI) == 0)
    {
        
    }
    else
    {
        data_points = (cluster_point *)malloc(sizeof(cluster_point) * c.datapoint_count);
        if (data_points == NULL )
        {
            ERROR_AND_EXIT();
        }
        list_reverse(&file_data);
        status = ioparser_parse_data_points(&c,&file_data,data_points);
        if(status != c.datapoint_count)
        {
            list_destroy(&file_data);
            cluster_point_destroy(data_points,status);
            ERROR_AND_EXIT();
        }
    }
    return 0;
}