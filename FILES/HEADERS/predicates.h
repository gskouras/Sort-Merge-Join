#ifndef _PREDICATES_H__
#define _PREDICATES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct 
{	
	int flag_exec; //flag to check if this predicate has been executed
	int rel1_alias; 
	int rel1_origin;
	int rel1_col;
	int rel2_alias;//an einai filtro to arxikopoio me -1
	int rel2_origin;
	int rel2_col;
	char op; // operation between the 2 columns
	uint64_t filter_value;
} Predicate;

typedef struct 
{
	Predicate *predicates_array;
	int size;
} Predicates;




#endif