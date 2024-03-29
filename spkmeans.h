#ifndef SPKMEANS
#define SPKMEANS
#include <stdbool.h>
#include "matrix.h"
#include "ioparser.h"
#include "cleanup.h"

#include <math.h>

#define WAM 101
#define DDG 201
#define GL 301

#define WAM_TXT "wam"
#define DDG_TXT "ddg"
#define GL_TXT "gl"
#define JACOBI_TXT "jacobi"

#define SPKMEANS_WAM_FORMULA(x,y,dim) (exp(-0.5*pow(vector_euclidean_distance(x,y,dim),2)))
#define SPKMEANS_THETA_FORMULA(m, loc) ((a->matrix[loc[1]][loc[1]] - a->matrix[loc[0]][loc[0]]) / (2 * a->matrix[loc[0]][loc[1]]))
#define SPKMEANS_SIGN(theta) (theta >= 0 ? 1 : -1)
#define SPKMEANS_T_FORMULA(theta) (SPKMEANS_SIGN(theta)/(fabs(theta) + sqrt(pow(theta, 2) + 1)))
#define SPKMEANS_C_FORMULA(t) (1 / sqrt(pow(t, 2) + 1))
#define SPKMEANS_MAX_ROATATIONS (100)
#define SWITCH(a,b) {a = a ^ b; b = a ^ b; a = a ^ b;}

typedef struct{
    char* pre_in;
    char* post_in;
    char* pre_out;
    char* post_out;
} pre_post;

matrix* spkmeans_wam(const  context*const,const point* const data_points);
matrix* spkmeans_ddg(const matrix* const m);
matrix* spkmeans_gl(const matrix* const wam,const matrix* const ddg);
void spkmeans_jacobi(matrix* m, matrix* ret[]);
#endif

