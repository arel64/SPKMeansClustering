#include "vector.h"

int vector_init_arr(point* arr, const unsigned k,const unsigned dim){
	size_t i;
	for (i = 0; i < k; i++)
	{
		arr[i] = (double *)calloc(dim, sizeof(double));
		if (arr[i] == NULL)
		{
			return i;
		}
	}
	return k;
}
double  vector_euclidean_distance(const point  a, const point b, const unsigned dim)
{
    double sum = 0;
	size_t i = 0;
	for (; i < dim; i++)
	{
		sum += pow((a[i] - b[i]), 2);
	}
	return sqrt(sum);
}
void vector_sum_into_vector(point *sumInto, const point*const sumFrom, const unsigned n)
{
	vector_each_cell(sumInto,sumInto,sumFrom,0,ADD_VECTOR,n);
}
void vector_multipy_vector_by_scalar   (point *vec, double scalar, const unsigned n)
{
	vector_each_cell(vec,vec,NULL,scalar,MULTIPLY_SCALAR,n);
}
double  vector_multiply (vector* vector_row,vector* vector_col, const unsigned dim)
{
	size_t i = 0;
	unsigned sum = 0;
	for (; i < dim; i++)
	{
		sum += (*vector_row)[i] * (*vector_col)[i];
	}
	return sum;
}

void vector_copyinto_vector(point *copyInto, const point*const copyFrom, const unsigned n)
{
	vector_each_cell(copyInto,copyFrom,NULL,0,COPYINTO_VECTOR,n);
}
void vector_each_cell(vector* into,const vector *const left,const vector *const right,const double scalar,unsigned how, const unsigned n)
{
    size_t i = 0;
    
    if(into == NULL) return;

    for(;i<n;i++)
    {
		switch (how)
		{
		case ADD_VECTOR:
			(*into)[i] = (*left)[i] + (*right)[i];
			break;
		case MULTIPLY_SCALAR:
			(*into)[i] = (*left)[i] * scalar;
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
void vector_destroy(vector **vecArr, unsigned int k)
{
	size_t j = 0;
	if(vecArr == NULL)
		return;
	for (; j < k; j++)
	{
		free((*vecArr)[j]);
		(*vecArr)[j] = NULL;
	}
	free(*vecArr);
	*vecArr = NULL;

}
