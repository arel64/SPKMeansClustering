#include "cluster_point.h"
void cluster_point_init_data_points_with_starting_cluster(const context*const c,cluster_point*cluster_points, centroid* cluster_centroids)
{
	size_t i;
	point_init_arr(c,cluster_centroids);
	for (i = 0; i < c->cluster_count; i++)
	{

		copyIntoPoint(&cluster_centroids[i], &cluster_points[i].point);
		cluster_points[i].belong = i + 1;
	}
}
void cluster_point_destroy_c(const context*const c,cluster_point *p)
{
	cluster_point_destroy(p,c->datapoint_count);
}
void cluster_point_destroy(cluster_point *p,const unsigned c)
{
	size_t i = 0;
	if (p != NULL)
	{
		for (i = 0; i < c; i++)
		{
			coordsDestroy(&p[i].point, 1);
		}
	}
	free(p);
}