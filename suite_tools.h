#ifndef SUITE_TOOLS
#define SUITE_TOOLS
#include <CUnit/Basic.h>
#include <CUnit/CUError.h>
CU_pSuite suite_tools_add_suite(char* name,int (*init)(void),int (*cleanup)(void));
#endif
