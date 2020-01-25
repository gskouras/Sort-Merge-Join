#include "../HEADERS/unitest.h"
#include "../HEADERS/list.h"

void TestJoin(CuTest *tc)
{
	relation input1;
	input1.tuples = malloc(sizeof(tuple));
	input1.tuples[1].key = 3;

	relation input2;
	input2.tuples = malloc(sizeof(tuple));
	input2.tuples[2].key = 3;

	//Test when 2 equal relation are given
	int actual = isEqual(&input1, &input2, 1, 2);
	int expected = 1;
	CuAssertIntEquals(tc, expected, actual);

}

void TestSort(CuTest *tc)
{
	relation input;
	input.tuples = malloc(sizeof(tuple) * 2);
	input.tuples[1].key = 5;
	input.tuples[2].key = 4;
	input.num_tuples = 2;

	//Test when 2 sorted numbers are given
	int actual = relation_checkifsorted(&input);
	int expected = 1;
	CuAssertIntEquals(tc, expected, actual);

}

void CheckRelTuples(CuTest *tc)
{
	relation input;
	input.tuples = malloc(sizeof(tuple)*3);
	input.num_tuples = 3;

	int actual = relation_getnumtuples(&input);
	int expected = 3;
	CuAssertIntEquals(tc, expected, actual);
}



CuSuite* EqualUtilGetSuite()
{
	CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestJoin);
    SUITE_ADD_TEST(suite, TestSort);
    SUITE_ADD_TEST(suite, CheckRelTuples);


    return suite;
}


void RunAllTest()
{
	CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
        
    CuSuiteAddSuite(suite, EqualUtilGetSuite());
    
    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    FILE *fp;
    fp = fopen("UnityTest.txt", "w");
    fprintf(fp, "%s\n", output->buffer);
    fclose(fp);
}

