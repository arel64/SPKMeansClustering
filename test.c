#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "testmatrix.h"
#include "testspkmeans.h"
int main(void)
{
   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */

   testmatrix_suite();
   testspkmeans_suite();
   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}