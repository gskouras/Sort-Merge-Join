#ifndef UNITEST_H
#define UNITEST_H

#include "CuTest.h"

CuSuite* EqualUtilGetSuite();

void TestJoin(CuTest *tc);

void TestSort(CuTest *tc);

void CheckRelTuples(CuTest *tc);

void RunAllTest();

#endif
