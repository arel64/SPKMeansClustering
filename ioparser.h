#ifndef IOPARSER
#define IOPARSER
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "linked_list.h"
#include "vector.h"

int ioparser_parse_file_linked_list         (       context*        ,linked_list *, FILE *stream);
int ioparser_parse_data_points              (const  context*const   ,linked_list *, point *);
point ioparser_parse_data_point             (const  context*const   ,char *);
void ioparser_print_final_centroids         (const  context*const   ,const centroid *);
point* ioparser_parse_file_to_data_points   (context*,char* filename);
int ioparser_get_dimention                  (const char* unparsed_line, ssize_t nnread_prev);
#endif
