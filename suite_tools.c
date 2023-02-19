#include "suite_tools.h"
CU_pSuite suite_tools_add_suite(char* name,int (*init)(void),int (*cleanup)(void))
{

    CU_pSuite pSuite  = CU_add_suite(name, init, cleanup);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return NULL;
    } 
    return pSuite;
    
}