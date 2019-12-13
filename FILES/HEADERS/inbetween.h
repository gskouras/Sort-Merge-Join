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


typedef struct 
{
	relation * rel1; 
	int rel1_origin;
	int rel1_col;
	int rel2_origin;
	int rel2_col;
} Inter_result;

typedef struct 
{
	Inter_result * inter_res_array;
	int size;
	int current;
} Inbetween_results;








#endif