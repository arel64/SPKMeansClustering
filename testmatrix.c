#include "testmatrix.h"
matrix* m0 = NULL;
matrix* m1 = NULL;
matrix* m2 = NULL;

void testCREATE(void)
{
    int i=0;
    m0= matrix_create(10, 5);
    CU_ASSERT_NOT_EQUAL_FATAL(m0,NULL);
    
    CU_ASSERT_EQUAL(m0->row,10);
    CU_ASSERT_EQUAL(m0->col,5);
    if(m0->matrix == NULL)
    {
        testmatrix_clean();
        CU_FAIL();
    }
    for(;i<10*5 - 1;i++)
    {
        CU_ASSERT_EQUAL(
            &m0->matrix[i%m0->col][i-(i%m0->row)] + 1,
            &m0->matrix[i%m0->col][i-(i%m0->row) + 1]
        );
    }

}
void testCREATEINVALID(void)
{
    m0 = matrix_create(0, 5);
    CU_ASSERT_EQUAL(m0,NULL);
    m0 = matrix_create(5,0);
    CU_ASSERT_EQUAL(m0,NULL);
    m0 = matrix_create(0,0);
    CU_ASSERT_EQUAL(m0,NULL);
    m0 = matrix_create(0,6);
    CU_ASSERT_EQUAL(m0,NULL);

}
void testMATRIXADD(void)
{
    m0 = matrix_create(3, 3);
    m1 = matrix_create(3, 3);
    CU_ASSERT_NOT_EQUAL(m0,NULL);
    CU_ASSERT_NOT_EQUAL(m1,NULL);
    if(m0 == NULL || m1 == NULL )
    {
        testmatrix_clean();
        CU_FAIL();
    }
    matrix_zerofill(m0);
    matrix_zerofill(m1);
    m0->matrix[0][1] = 5;
    m1->matrix[0][1] = 4;
    m0->matrix[0][2] = 100;
    m2 = matrix_add(m0, m1);
    if(m2 == NULL)
    {
        testmatrix_clean();
        CU_FAIL();
    }
    CU_ASSERT_EQUAL(m2->matrix[0][2], 100);
    CU_ASSERT_EQUAL(m2->matrix[0][1], 9);
    CU_ASSERT_EQUAL(m2->matrix[0][0], 0);
    testmatrix_clean();
}
void testMATRIXMULTIPLYSCALAR(void)
{
    double scalar =3.5;
    m0 = matrix_create(3, 3);
    CU_ASSERT_NOT_EQUAL(m0,NULL);
    if(m0 == NULL )
    {
        testmatrix_clean();
        CU_FAIL();
    }
    matrix_zerofill(m0);
    m0->matrix[0][1] = 5;
    m0->matrix[0][2] = 100;
    m0->matrix[2][2] = 100;
    m1 = matrix_multiply_scalar(m0, scalar);
    if(m1 == NULL )
    {
        testmatrix_clean();
        CU_FAIL();
    }
    CU_ASSERT_DOUBLE_EQUAL(m1->matrix[0][1], scalar*5,0.1);
    CU_ASSERT_DOUBLE_EQUAL(m1->matrix[0][2], scalar*100,0.1);
    CU_ASSERT_DOUBLE_EQUAL(m1->matrix[2][2], scalar*100,0.1);

    testmatrix_clean();

}
void testMATRIXMULTIPLY(void)
{
    m0 = matrix_create(3, 3);
    m1 = matrix_create(3, 2);
    CU_ASSERT_NOT_EQUAL(m0,NULL);
    CU_ASSERT_NOT_EQUAL(m1,NULL);
    if(m0 == NULL || m1 == NULL )
    {
        testmatrix_clean();
        CU_FAIL();
    }
    m0->matrix[0][0] = 1;
    m0->matrix[0][1] = 3;
    m0->matrix[0][2] = 4;
    m0->matrix[1][0] = 41;
    m0->matrix[1][1] = 12;
    m0->matrix[1][2] = 3.5;
    m0->matrix[2][0] = 1.2;
    m0->matrix[2][1] = 3.3;
    m0->matrix[2][2] = 1.1;
    m1->matrix[0][0] = 1.8;
    m1->matrix[0][1] = 3.1;
    m1->matrix[1][0] = 4144;
    m1->matrix[1][1] = 12.1;
    m1->matrix[2][0] = 1.222;
    m1->matrix[2][1] = 3.33;
    m2 = matrix_multiply(m0,m1);
    if(m2 == NULL )
    {
        testmatrix_clean();
        CU_FAIL();
    }
    CU_ASSERT_EQUAL(m2->row,m0->col);
    CU_ASSERT_EQUAL(m2->col,m1->col);
    if(m2->row != m0->col || m2->col != m1->col)
    {
        testmatrix_clean();
        CU_FAIL();
    }
    
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[0][0],12438.688,0.01);
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[0][1],52.72,0.01);
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[1][0],49806.077,0.01);
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[1][1],283.955,0.01);
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[2][0],13678.7042,0.01);
    CU_ASSERT_DOUBLE_EQUAL(m2->matrix[2][1],47.313,0.01);
    testmatrix_clean();

}


int testmatrix_init(void)
{
    return 0;
}
int testmatrix_clean(void)
{
    free(m0);
    free(m1);
    free(m2);
    m0 = NULL;
    m1 = NULL;
    m2 = NULL;
    return 0;
}
CU_pSuite testmatrix_suite(void)
{
  
    CU_pSuite pSuite = suite_tools_add_suite("matrix",testmatrix_init,testmatrix_clean);
    if(pSuite == NULL)
    {
        return NULL;
    }
    if (
        (NULL == CU_add_test(pSuite, "test of matrix_create()", testCREATE)) ||
        (NULL == CU_add_test(pSuite, "test of matrix_create() Invalid", testCREATEINVALID))||
        (NULL == CU_add_test(pSuite, "test of matrix_add()", testMATRIXADD))||
        (NULL == CU_add_test(pSuite, "test of matrix_multipyscalar()", testMATRIXMULTIPLYSCALAR))||
        (NULL == CU_add_test(pSuite, "test of matrix_multipy()", testMATRIXMULTIPLY))
    )
    {
        CU_cleanup_registry();
        return NULL;
    }

    return pSuite;
}

