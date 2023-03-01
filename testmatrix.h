#ifndef TESTMATRIX
#define TESTMATRIX
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "suite_tools.h"
#include "./matrix.h"

int testmatrix_init(void);
int testmatrix_clean(void);
CU_pSuite testmatrix_suite(void);


void testCREATE(void);
void testCREATEINVALID(void);
void testMATRIXADD(void);
void testMATRIXMULTIPLYSCALAR(void);
void testMATRIXMULTIPLY(void);

extern matrix* m0;
extern matrix* m1;
extern matrix* m2;
#endif
