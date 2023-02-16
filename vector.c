#include "vector.h"

int vector_init_arr(const context*const c,point* arr){
	size_t i;
	for (i = 0; i < c->cluster_count; i++)
	{
		arr[i] = (double *)calloc(c->dimention, sizeof(double));
		if (arr[i] == NULL)
		{
			return i;
		}
	}
	return c->cluster_count;
}
double  vector_euclidean_distance(const context*const c,const point * a, const point*b)
{
    double sum = 0;
	size_t i = 0;
	for (; i < c->dimention; i++)
	{
		sum += pow((a[i] - b[i]), 2);
	}
	return sqrt(sum);
}
void vector_sum_into_vector(const context*const c,point *sumInto, const point*const sumFrom)
{
	vector_each_cell(c,sumInto,sumInto,sumFrom,0,ADD_VECTOR);
}
void vector_multipy_vector_by_scalar   (const context*const c ,const point *vec, double scalar)
{
	vector_each_cell(c,vec,NULL,NULL,scalar,MULTIPLY_SCALAR);
}
void vector_copyinto_vector(const context*const c,point *copyInto, const point*const copyFrom)
{
	vector_each_cell(c,copyInto,copyFrom,NULL,0,COPYINTO_VECTOR);
}
void vector_each_cell(const context*const c,vector* into,const vector *const left,const vector *const right,const double scalar,unsigned how)
{
    size_t i = 0 , j = 0;
    size_t n = c->datapoint_size;
    
    if(into == NULL) return;

    for(;i<n;i++)
    {
		switch (how)
		{
		case ADD_VECTOR:
			(*into)[i] = (*left)[i] + (*right)[i];
			break;
		case MULTIPLY_SCALAR:
			(*into)[i] = (*into)[i] * scalar;
			break;
		case COPYINTO_VECTOR:
			(*into)[i] = (*left)[i];
			break;
		default:
			return;
			break;
		}
    }
}
void vec_destroy_c(const context*const c,vector *vecArr)
{
	vec_destroy(vecArr,c->datapoint_size);
}
void vec_destroy(vector *vecArr, unsigned int k)
{
	if(vecArr == NULL)
		return;
	size_t j = 0;
	if (vecArr == NULL)
	{
		return;
	}
	for (; j < k; j++)
	{
		free(vecArr[j]);
	}
}