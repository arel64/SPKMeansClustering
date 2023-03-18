#include "ioparser.h"
int ioparser_parse_file_linked_list(context* c,linked_list *l, FILE *stream)
{
    char *datapoint_line = NULL;
    size_t lines_read = 0;
	size_t length = 0;
	ssize_t nread;
	while ((nread = getline(&datapoint_line, &length, stream)) != EOF)
	
	{
		char *temp = (char *)malloc(sizeof(char) * nread);
		if (temp == NULL)
		{
			return 1;
		}
		strncpy(temp, datapoint_line,nread);
		if (!list_insert_key(l, temp))
		{
			free(temp);
			return 1;
		}
		lines_read++;
	}
	free(datapoint_line);
    c->dimention = ioparser_get_dimention(l->head->key);
    c->datapoint_count = lines_read;
	return 0;
}

int ioparser_parse_data_points(const context*const c,linked_list *l , point *data)
{
	double *temp;
    size_t lines_parsed = 0;
	list_node *iter = l->head;

	while (iter != NULL)
	{
		temp = ioparser_parse_data_point(c,iter->key);
		if (temp == NULL)
		{
			return lines_parsed;
		}
		*data = temp;
		data++;
		iter = iter->next;
        lines_parsed++;
	}

	return lines_parsed;
}
point ioparser_parse_data_point(const context*const c,char *line)
{
	char *endI = NULL;
	size_t i = 0;
	double *ret = malloc(sizeof(double) * c->dimention);
	if (ret == NULL)
	{
		return NULL;
	}
	while (endI == NULL || (*endI != EOF && *endI != '\n'))
	{
		ret[i] = strtod(line, &endI);
		line += (endI - line) + 1;
		i++;

	}
	return ret;
}
void ioparser_print_final_centroids(const context*const c,const centroid * final_centroids)
{
	size_t i, j;
	for (i = 0; i < c->cluster_count; i++)
	{
		for (j = 0; j < c->dimention; j++)
		{
			printf("%.4f", final_centroids[i][j]);
			if (j != c->dimention - 1)
			{
				printf(",");
			}
		}
		printf("\n");
	}
}

int ioparser_get_dimention(const char* unparsed_line)
{
	int i = 0;
	int count = 0; /*counting of the char ',' in the given string.*/
	while((*(unparsed_line + i) != '\n') && (*(unparsed_line + i) != EOF)){
		if(*(unparsed_line + i) == ',')
			count++;
		i++;
	}
	return count + 1;
}
point* ioparser_parse_file_to_data_points   (context* c,char* file_name)
{
	linked_list file_data;
	FILE* file;
    vector* data_vecs;
    unsigned int status;


	list_init(&file_data);
	/*
        Open File
    */
    file = fopen(file_name,"r");
    if(file == NULL)
    {
        return NULL;	
    }
   

    /* 
        Get all data in string form
    */
	if (ioparser_parse_file_linked_list(c,&file_data, file))
	{
        list_destroy(&file_data);
        fclose(file);
         return NULL;	
	}
    fclose(file);
    data_vecs = (vector *)malloc(sizeof(vector) * c->datapoint_count);
    if (data_vecs == NULL )
    {
        list_destroy(&file_data);
         return NULL;	
    }
    
    list_reverse(&file_data); 
    status = ioparser_parse_data_points(c,&file_data,data_vecs); 
    list_destroy(&file_data);
    if(status != c->datapoint_count)
    {
        vector_destroy(data_vecs,status);
 		return NULL;	
    }
	return data_vecs;
}
