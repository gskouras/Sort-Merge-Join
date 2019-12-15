#ifndef _BATCH_H
#define _BATCH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/relation.h"


typedef struct
{
	char **batch; //pointer to each query of a bach
	int size; // size of each bach
} Batch_lines;


typedef struct
{
	Batch_lines *batches; //pointer to each bach
	int size; //size of all batches
} Batches;


typedef struct 
{	
	int flag_exec; //flag to check if this predicate has been executed
	int rel1_alias; // alias name of relation 1 in the query 
	int rel1_origin; //actual name of first relation 
	int rel1_col; //column of the relation 1
	int rel2_alias;//an einai filtro to arxikopoio me -1 //// alias name of relation 1 in the query 
	int rel2_origin; //actual name of second relation 
	int rel2_col;//column of the relation 2
	char op; // operation between the 2 columns
	uint64_t filter_value; 
} Predicate;

typedef struct 
{
	Predicate *predicates_array; // array that holds the predicates of current query
	int size; // number of predicates of current query
} Predicates;


typedef struct
{
	int rel_alias; //alias name of the relations to be projected
	int rel_origin; //actual name of the relation to be projected
	int rel_col; //column of te relation
} Check_sum;

typedef struct
{	
	Check_sum * check_sums_array; //array that hold the projections of current query
	int size; //number of projections of current query;
 }Check_sums;



void fillBatches(Batches *batches, char *s, char *filename);

void free_all_batches(Batches * batches); // free all memory allocated to hold the information for batches

Batches * createBatches(char *filename, char *s);

void execute_all_batches(char* filename, all_data * datatable); //read all batched from the file

void execute_batch(Batch_lines * batch_lines, all_data * datatable ); //execute each batch seperately

void execute_query(char * query, all_data *datatable); //execute each query seperately

int find_num_of_relations(char * rel_line); //calculating number of relations participating in a query

int find_num_of_predicates(char * pred_line); //calculating number of predidcates participating in a query

void fill_predicates(char *pred_str, Predicates *pd, int *alias_array); //fill the array that holds the predicates

void fill_relations(char *rel_str, int* alias_array); //fill the array that hold the relations

void fill_check_sums(char *check_sum_str, Check_sums *cs, int * alias_array);////fill the array that holds the projections//check_sums

int isnumber(char * number); //checking if a given string is number

void print_check_sums(Check_sums * cs, all_data *datatable);






#endif