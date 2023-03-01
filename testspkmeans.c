#include "testspkmeans.h"

void testWAM(void)
{
    matrix* ret;
    matrix* m = matrix_create(7,5);
    context c = {5,0,7};
    double row1[]={1.0, 2.0, 3.0, 4.0, 5.0};
    double row2[]={2.0, 4.0, 6.0, 8.0, 10.0};
    double row3[]={3.0, 6.0, 9.0, 12.0, 15.0};
    double row4[]={4.0, 8.0, 12.0, 16.0, 20.0};
    double row5[]={5.0, 10.0, 15.0, 20.0, 25.0};
    double row6[]={6.0, 12.0, 18.0, 24.0, 30.0};
    double row7[]={7.0, 14.0, 21.0, 28.0, 35.0};
    
    m->matrix[0] = row1;
    m->matrix[1] = row2;
    m->matrix[2] = row3;
    m->matrix[3] = row4;
    m->matrix[4] = row5;
    m->matrix[5] = row6;
    m->matrix[6] = row7;
    ret = spkmeans_wam(&c,m->matrix);

    free(m);
    free(ret);
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
    CU_FAIL();
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

