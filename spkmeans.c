#include "spkmeans.h"
#define WAM_FORMULA(c,x,y) (exp(-0.5*vector_euclidean_distance(c,x,y)))
matrix* spkmeans_wam(const  context*const c,const point* const data_points)
{
    size_t i=0,j=0;
    size_t n=c->datapoint_count;
    matrix* ret = matrix_create(c);
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
                (*ret)[i][j] = 0;
            }
            else if(i<j)
            {
                (*ret)[i][j] = WAM_FORMULA(c,&data_points[i],&data_points[j]);
            }
            else
            {
                (*ret)[i][j] =  (*ret)[j][i];
            }
        }
    }
    return ret;
}
