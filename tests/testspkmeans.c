#include "testspkmeans.h"
#include <CUnit/CUnit.h>
#include <string.h>
#define TESTSPKMEANS_MAGIC_TEST_NUMBER 10
#define FILE_IN_PREFIX_WAM "tests_input/test"
#define FILE_IN_SUFFIX_WAM ".txt"
#define FILE_OUT_PREFIX_WAM "tests_output/C/test"
#define FILE_OUT_SUFFIX_WAM "_wam.txt"

#define FILE_IN_PREFIX_DDG "tests_input/test"
#define FILE_IN_SUFFIX_DDG ".txt"
#define FILE_OUT_PREFIX_DDG "tests_output/C/test"
#define FILE_OUT_SUFFIX_DDG "_ddg.txt"

#define FILE_IN_PREFIX_GL "tests_input/test"
#define FILE_IN_SUFFIX_GL ".txt"
#define FILE_OUT_PREFIX_GL "tests_output/C/test"
#define FILE_OUT_SUFFIX_GL "_gl.txt"

#define WAM 0
#define DDG 1
#define GL 2


void imposeInt(int n,char* out,char* pre,char* post)
{
    char test_count_str[255];
    strcpy(out, pre);
    sprintf(test_count_str, "%d", n);
    strcat(out,test_count_str );
    strcat(out,post );
}
void test_matrix_function(int func,pre_post* p)
{
    vector *computed_vecs,*expected_vecs;
    context c_computed = {0,0,0};
    context c_expected = {0,0,0};
    matrix *computed_matrix= NULL,*expected_matrix = NULL;
    matrix *temp_wam = NULL,*temp_ddg =NULL ;
    int test_count = 1;
    unsigned n_computed,n_expected;
    for(;test_count<TESTSPKMEANS_MAGIC_TEST_NUMBER;test_count++)
    {
       
        char* file_in_name  = malloc(sizeof(char)*255);
        char* file_out_name = malloc(sizeof(char)*255);
        imposeInt(test_count, file_in_name,p->pre_in,p->post_in);
        if(file_in_name == NULL)
        {
            CU_FAIL_FATAL();
        }
        imposeInt(test_count, file_out_name,p->pre_out,p->post_out);
        if(file_out_name == NULL)
        {
            free(file_in_name);
            CU_FAIL_FATAL();
        }
        computed_vecs = ioparser_parse_file_to_data_points   (&c_computed,file_in_name);
        if(computed_vecs == NULL)
        {
            CU_FAIL_FATAL();
        }
        expected_vecs  = ioparser_parse_file_to_data_points   (&c_expected,file_out_name);
        if(expected_vecs == NULL)
        {
            vector_destroy(computed_vecs, c_expected.datapoint_count);
            CU_FAIL_FATAL();
        }
        n_expected = c_expected.datapoint_count;
        n_computed = c_computed.datapoint_count;
        switch(func)
        {
            case WAM:
                computed_matrix = spkmeans_wam(&c_computed,computed_vecs);

            break;
            case DDG:
                temp_wam = spkmeans_wam(&c_computed,computed_vecs);
                if(temp_wam == NULL)
                {
                    CU_FAIL_FATAL();
                }
                computed_matrix = spkmeans_ddg(temp_wam);

            break;
            case GL:
                temp_wam = spkmeans_wam(&c_computed,computed_vecs);
                if(temp_wam == NULL)
                {
                    CU_FAIL_FATAL();
                }
                temp_ddg = spkmeans_ddg(temp_wam);
                if(temp_ddg == NULL)
                {
                    CU_FAIL_FATAL();
                }
                computed_matrix = spkmeans_gl(temp_wam,temp_ddg);
            break;
            default:
                CU_FAIL_FATAL();
            break;
        }
        if(computed_matrix == NULL)
        {
            CU_FAIL_FATAL();
        }
        expected_matrix = matrix_create_from_data(n_expected, n_expected,expected_vecs );
        if(computed_matrix == NULL)
        {
            matrix_destroy(computed_matrix);
            CU_FAIL_FATAL();
        }
        /*
            This section is horrificly inefficent, but this is a test so... Duck it
        */
        vector_destroy(expected_vecs, n_expected);
        vector_destroy(computed_vecs, n_computed);
        CU_ASSERT_EQUAL(matrix_is_equal(computed_matrix,expected_matrix,0.00009),1);
        matrix_destroy(computed_matrix);
        matrix_destroy(expected_matrix);
    }
}
void testWAM(void)
{
    pre_post p = {FILE_IN_PREFIX_WAM,FILE_IN_SUFFIX_WAM,FILE_OUT_PREFIX_WAM,FILE_OUT_SUFFIX_WAM};
    test_matrix_function(WAM,&p);

}
void testDDG(void)
{
    pre_post p = {FILE_IN_PREFIX_DDG,FILE_IN_SUFFIX_DDG,FILE_OUT_PREFIX_DDG,FILE_OUT_SUFFIX_DDG};
    test_matrix_function(DDG,&p);
}
void testLP(void)
{
   pre_post p = {FILE_IN_PREFIX_GL,FILE_IN_SUFFIX_GL,FILE_OUT_PREFIX_GL,FILE_OUT_SUFFIX_GL};
    test_matrix_function(GL,&p);
}
void testJACOBI(void)
{
   matrix *m;
   matrix* dual[2] = {NULL,NULL};
   m = matrix_create(3, 3);
   m->matrix[0][0] = 2;
   m->matrix[0][1] = -1;
   m->matrix[0][2] = 0;

   m->matrix[1][0] = -1;
   m->matrix[1][1] = 2;
   m->matrix[1][2] = -1;

   m->matrix[2][0] = 0;
   m->matrix[2][1] = -1;
   m->matrix[2][2] = 2;

    matrix_print(m);
   spkmeans_jacobi(m,dual);
   matrix_print(dual[0]);
   printf("\n");
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

