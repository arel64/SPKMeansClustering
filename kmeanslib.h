#ifndef KMEANSLIB
#define KMEANSLIB
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct point
{
	double *coords;
	unsigned int dimention;
	/*
		TODO :: This is a waste of space, change it
	*/
} Point;
typedef Point Centroid;
typedef struct clusterPoint
{
	Point point;
	unsigned int belong;
} ClusterPoint;


Centroid* fit(
	ClusterPoint *dataPoints,const unsigned int dataPointsSize,
	Centroid* initialCentroids,const unsigned int centroidsSize,
	unsigned int iter,double eps
	);
void parseDataPoint(char *, double *outData);
int extractD(char *);
/*
	Actual logic
*/
void initDataPointsWithStartingCluster(ClusterPoint *, Centroid *, unsigned int);
Point* initPointArr(Point *, unsigned int, unsigned int);
void updateDataPointCluster(ClusterPoint *data, const unsigned int dataSize, Centroid *centeroids, const unsigned int centeroidsSize);
int mmDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize, const int operator);
int minDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize);
int maxDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize);
double euclideanDistance(const Point *a, const Point *b);
void sumIntoPoint(Point *a, const Point *b);
void multipyPointByScalar(const Point *p, double scalar);
void copyIntoPoint(Point *copyInto, const Point *copyFrom);
int getUpdatedCentroids(const ClusterPoint *clusterPoints, const unsigned int n, const unsigned int k, Centroid *updatedCentroids);
/*
	Error handeling and cleanup
*/

void centroidsDestroy(Centroid *, unsigned int k);
void pointDestroy(Point *, unsigned int k);
void coordsDestroy(Point *pArr, unsigned int k);
void clusterPointDestroy(ClusterPoint *c, unsigned int n);
void cleanup(ClusterPoint **dataPoints,Centroid **centroids,Centroid **updatedCentroids,int n,int k);

#endif