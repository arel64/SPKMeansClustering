#ifndef CLUSTERPOINT
#define CLUSTERPOINT
#include "vector.h"
typedef struct clusterPoint
{
	point point;
	unsigned int belong;
} cluster_point;
void cluster_point_init_data_points_with_starting_cluster(const context*const,cluster_point*,centroid*);
void cluster_point_destroy_c(const context*const c,cluster_point *p);
void cluster_point_destroy(cluster_point *p,const unsigned);

#endif