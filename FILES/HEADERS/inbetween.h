#ifndef _BETWEEN_H_
#define _BETWEEN_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/relation.h"
#include "../HEADERS/batch.h"
#include "../HEADERS/sort_join.h"


typedef struct 
{
	relation * rel; 
	int rel1_origin;
	int rel1_alias;
	int rel1_col;
	int rel2_origin;
	int rel2_alias;
	int rel2_col;
} Inbet_result;

typedef struct 
{
	Inbet_result * inbet_res_array;
	int size;
	int current;
} Inbetween_results;


void execute_predicates(Predicates *pd, all_data *dt);

int calc_tuples_size_after_filter(Predicates *pd, all_data *dt);

relation * execute_filters(Predicates * pd, all_data * dt, relation* updated_rel, int num_of_tuples);







#endif