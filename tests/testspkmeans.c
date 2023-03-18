#include "testspkmeans.h"

void testWAM(void)
{
    CU_FAIL();
/*
    matrix* ret;
    matrix* m = matrix_create(7,5);
    context c = {5,0,7};
    size_t i=0,j=0;
    for(;i<7;i++)
    {
        for(j=0;j<5;j++)
        {
            m->matrix[i][j] = i+1;
        }
    }
    ret = spkmeans_wam(&c,m->matrix);
    matrix_print(ret);
    matrix_destroy(m);
    matrix_destroy(ret);
    */
}
void testDDG(void)
{
    CU_FAIL();

}
void testLP(void)
{
    CU_FAIL();

}
void testJACOBI(void)
{
    /*
    matrix* dual [2]={NULL,NULL};
    matrix* m0 = matrix_create(3,3);
    matrix* m10 = matrix_create(3,3);
    matrix* m11 = matrix_create(1,3);
    dual[0] = m10;
    dual[1] = m11;

    m0->matrix[0][0] = 1;
    m0->matrix[0][1] = 2;
    m0->matrix[0][2] = 3;
    
    m0->matrix[1][0] = 2;
    m0->matrix[1][1] = 1;
    m0->matrix[1][2] = 4;

    m0->matrix[2][0] = 3;
    m0->matrix[2][1] = 4;
    m0->matrix[2][2] = 5;
    matrix_print(m0);
    spkmeans_jacobi(m0,dual);
    matrix_print(dual[0]);
    matrix_print(dual[1]);
    */
    matrix* dual [2]={NULL,NULL};
    matrix* m0 = matrix_create(2,2);
    matrix* m10 = matrix_create(2,2);
    matrix* m11 = matrix_create(1,2);
    dual[0] = m10;
    dual[1] = m11;
    m0->matrix[0][0] = 2;
    m0->matrix[0][1] = 3;

    m0->matrix[1][0] = 3;
    m0->matrix[1][1] = 7;
    spkmeans_jacobi(m0,dual);
    matrix_print(dual[0]);
    matrix_print(dual[1]);

}
int testspkmeans_init(void)
{
    return 0;
}
int testspkmeans_clean(void)
{
    return 0;
}
CU_pSuite testspkmeans_suite(void)
{
  
    CU_pSuite pSuite = suite_tools_add_suite("api",testspkmeans_init,testspkmeans_clean);
    if(pSuite == NULL)
    {
        return NULL;
    }
    if (
        (NULL == CU_add_test(pSuite, "test of WAM()", testWAM))||
        (NULL == CU_add_test(pSuite, "test of DDG()", testDDG))||
        (NULL == CU_add_test(pSuite, "test of LP()", testLP))||
        (NULL == CU_add_test(pSuite, "test of JACOBI()", testJACOBI))
    )
    {
        CU_cleanup_registry();
        return NULL;
    }

    return pSuite;
}

