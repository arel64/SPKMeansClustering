#include "spkmeans.h"
#define WAM_FORMULA(x,y,dim) (exp(-0.5*pow(vector_euclidean_distance(x,y,dim),2)))
matrix* spkmeans_wam(const  context*const c,const point* const data_points)
{
    size_t i=0,j=0;
    size_t n=c->datapoint_count;
    matrix* ret = matrix_create(c->datapoint_count,c->datapoint_count);
    if(ret==NULL)
    {
        return NULL;
    }
    for(;i<n;i++)
    {
        for(;j<n;j++)
        {
            if(i==j)
            {
                ret->matrix[i][j] = 0;
            }
            else if(i<j) 
            /*
             May be problematic because the else part can be excuted prior to the else if part.
            */
            {
                ret->matrix[i][j] = WAM_FORMULA(data_points[i],data_points[j],c->dimention);
            }
            else
            {
                ret->matrix[i][j] =  ret->matrix[j][i];
            }
        }
    }
    return ret;
}

matrix* spkmeans_ddg(const matrix* const wam)
{
    size_t i=0;
    unsigned size = wam->row;
    matrix* ret = matrix_create(size,size);
    if(ret==NULL)
    {
        return NULL;
    }
    matrix_zerofill(ret);
    for(;i<size;i++)
    {
        ret->matrix[i][i] =  matrix_sum_row(wam,i);
        
    }
    return ret;
}
matrix* spkmeans_gl(const matrix* const wam,const matrix* const ddg)
{
    return matrix_subtract(ddg, wam);
}
