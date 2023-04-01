#include "kmeanslib.h"

Centroid *updatedCentroids = NULL;
Centroid* fit(
	ClusterPoint *dataPoints,const unsigned int n,
	Centroid* centroids,const unsigned int k,
	unsigned int iter,double eps
	)
{
	size_t i = 0;
	unsigned int itersCompleted = 0;
	unsigned int converged = 0;
	double maxDelta;
	double tempDelta;
	updatedCentroids = (Centroid *)malloc(sizeof(Centroid) * k);
	if (updatedCentroids == NULL)
	{
		cleanup(&dataPoints,&centroids,&updatedCentroids,n,k);
		return NULL;
	}
	updatedCentroids = initPointArr(updatedCentroids, dataPoints->point.dimention, k);
	
    /*
        Start Logic 
    */
	while (!converged && itersCompleted < iter)
	{
		/*
			Assign every xi to the closest cluster
		*/
		updateDataPointCluster(dataPoints, n, centroids, k);

		/*
			updatedCentroids are updated here
		*/
		if (getUpdatedCentroids(dataPoints, n, k, updatedCentroids) == 1)
		{
			cleanup(&dataPoints,&centroids,&updatedCentroids,n,k);
			return NULL;
		}

		/*
			Check Convergence
		*/
		maxDelta = 0;
		for (i = 0; i < k; i++)
		{
			tempDelta = euclideanDistance(&updatedCentroids[i], &centroids[i]);
			if (tempDelta > maxDelta)
			{
				maxDelta = tempDelta;
			}
		}

		if (maxDelta < eps)
		{
			converged = 1;
		}
		for (i = 0; i < k; i++)
		{
			copyIntoPoint(&centroids[i], &updatedCentroids[i]);
		}

		itersCompleted++;
	}
	/*
		Finished, Cleanup
	*/
	clusterPointDestroy(dataPoints, n);
	centroidsDestroy(updatedCentroids, k);
	dataPoints = NULL;
	updatedCentroids = NULL;
	/*
		Return
	*/
	return centroids;
}
void updateDataPointCluster(ClusterPoint *data, const unsigned int dataSize, Centroid *centeroids, const unsigned int centeroidsSize)
{
	size_t i;
	int minCluster;
	for (i = 0; i < dataSize; i++)
	{
		minCluster = minDistanceIndex(&data[i].point, centeroids, centeroidsSize) + 1;
		data[i].belong = minCluster;
	}
}
int minDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize)
{
	return mmDistanceIndex(point, otherPoints, otherPointsSize, 0);
}
int maxDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize)
{
	return mmDistanceIndex(point, otherPoints, otherPointsSize, 1);
}
int mmDistanceIndex(const Point *point, const Point *otherPoints, const unsigned int otherPointsSize, const int operator)
{
	size_t i;
	unsigned int minIndex = 0;
	double minDistance = euclideanDistance(point, &otherPoints[0]);
	double tempDistance;
	for (i = 1; i < otherPointsSize; i++)
	{
		tempDistance = euclideanDistance(point, &otherPoints[i]);
		if (operator== 0 ? (tempDistance < minDistance) : (tempDistance > minDistance))
		{
			minIndex = i;
			minDistance = tempDistance;
		}
	}
	return minIndex;
}
double euclideanDistance(const Point *a, const Point *b)
{
	double sum = 0;
	size_t i = 0;
	for (; i < a->dimention; i++)
	{
		sum += pow((a->coords[i] - b->coords[i]), 2);
	}
	return sqrt(sum);
}
int getUpdatedCentroids(const ClusterPoint *clusterPoints, const unsigned int n, const unsigned int k, Centroid *updatedCentroids)
{

	size_t i;
	unsigned int dim = clusterPoints[0].point.dimention;
	int *clusterSizeArr = (int *)calloc(k, sizeof(int));
	Point * clusterSumArr = (Point *)calloc(sizeof(Centroid) , k);

	

	if (updatedCentroids == NULL || clusterSizeArr == NULL || clusterSumArr == NULL)
	{
		free(clusterSizeArr);
		free(clusterSumArr);
		return 1;
	}

	clusterSumArr = initPointArr(clusterSumArr, dim, k);
	if(clusterSumArr == NULL)
	{
		free(clusterSizeArr);
		return 1;
	}
	for (i = 0; i < n; i++)
	{
		clusterSizeArr[clusterPoints[i].belong - 1] += 1;
		sumIntoPoint(&clusterSumArr[clusterPoints[i].belong - 1], &clusterPoints[i].point);
	}

	for (i = 0; i < k; i++)
	{
		if (clusterSizeArr[i] == 0)
		{
			free(clusterSizeArr);
			free(clusterSumArr);
			return 1;
		}
		multipyPointByScalar(&clusterSumArr[i], 1 / ((double)clusterSizeArr[i]));
		copyIntoPoint(&updatedCentroids[i], &clusterSumArr[i]);
	}
	pointDestroy(clusterSumArr, k);
	free(clusterSizeArr);
	return 0;
}
void sumIntoPoint(Point *sumInto, const Point *sumFrom)
{
	size_t i;
	for (i = 0; i < sumInto->dimention; i++)
	{
		sumInto->coords[i] += sumFrom->coords[i];
	}
}
void multipyPointByScalar(const Point *p, double scalar)
{
	size_t i;
	for (i = 0; i < p->dimention; i++)
	{
		p->coords[i] *= scalar;
	}
}
void copyIntoPoint(Point *copyInto, const Point *copyFrom)
{
	size_t i;
	for (i = 0; i < copyInto->dimention; i++)
	{
		copyInto->coords[i] = copyFrom->coords[i];
	}
}

Point* initPointArr(Point *arr, unsigned int dim, unsigned int k)
{
	size_t i;
	for (i = 0; i < k; i++)
	{
		arr[i].coords = (double *)calloc(dim, sizeof(double));
		arr[i].dimention = dim;
		if (arr[i].coords == NULL)
		{
			pointDestroy(arr,i);
			return NULL;
		}
	}
	return arr;
}
void centroidsDestroy(Centroid *c, unsigned int k)
{
	pointDestroy(c, k);
}
void pointDestroy(Point *pArr, unsigned int k)
{
	coordsDestroy(pArr, k);
	free(pArr);
}
void coordsDestroy(Point *pArr, unsigned int k)
{
	size_t j = 0;
	if(pArr == NULL)
		return;
	if (pArr == NULL)
	{
		return;
	}
	for (; j < k; j++)
	{
		free(pArr[j].coords);
	}
}
void clusterPointDestroy(ClusterPoint *c, unsigned int n)
{
	size_t i = 0;
	if (c != NULL)
	{
		for (i = 0; i < n; i++)
		{
			coordsDestroy(&c[i].point, 1);
		}
	}
	free(c);
}
void cleanup(ClusterPoint **dataPoints,Centroid **centroids,Centroid **updatedCentroids,int n,int k)
{
	clusterPointDestroy(*dataPoints, n);
	centroidsDestroy(*centroids, k);
	centroidsDestroy(*updatedCentroids, k);
	*dataPoints = NULL;
	*centroids = NULL;
	*updatedCentroids = NULL;
}