#ifndef _BETWEEN_H_
#define _BETWEEN_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/predicates.h"
#include "../HEADERS/relation.h"


typedef struct 
{
	relation * rel;
	int rel1_alias; 
	int rel1_origin;
	int rel1_col;
	int rel2_alias;
	int rel2_origin;
	int rel2_col;
} Inter_result;

typedef struct 
{
	Inter_result * inter_res;
	int size;
	int current;
} Bet_results;




void execute_filters(Predicates * pd, all_data * datatable);



#endif