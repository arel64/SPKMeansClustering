#ifndef IOPARSER
#define IOPARSER
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "linked_list.h"
#include "vector.h"

int ioparser_parse_file_linked_list (       context*        ,linked_list *, FILE *stream);
int ioparser_parse_data_points      (const  context*const   ,linked_list *, point *);
point ioparser_parse_data_point     (const  context*const   ,char *);
void ioparser_print_final_centroids (const  context*const   ,const centroid *);
#endif
