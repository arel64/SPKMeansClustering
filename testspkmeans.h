#ifndef TESTSPKMEANSAPI
#define TESTSPKMEANSAPI
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "suite_tools.h"
#include "./spkmeans.h"

int testapi_init(void);
int testapi_clean(void);
CU_pSuite testspkmeans_suite(void);


void testWAM(void);
void testDDG(void);
void testLP(void);
void testJACOBI(void);

#endif
