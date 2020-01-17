#ifndef _QUERY_H
#define _QUERY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/relation.h"
#include "../HEADERS/result_list.h"

#define M 5000000

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

char *execute_query ( Predicates * , Check_sums * , all_data * );

int find_num_of_relations(char * rel_line); //calculating number of relations participating in a query

int find_num_of_predicates(char * pred_line); //calculating number of predidcates participating in a query

void fill_predicates(char *pred_str, Predicates *pd, int *alias_array); //fill the array that holds the predicates

void fill_relations(char *rel_str, int* alias_array); //fill the array that hold the relations

void fill_check_sums(char *check_sum_str, Check_sums *cs, int * alias_array);////fill the array that holds the projections//check_sums

int isnumber(char * number); //checking if a given string is number

float instead_of_pow(float a, float b);

void restore_statistics (relation_data ** relations, int rel);

float update_statistics(relation_data ** relations, Predicate ** predicate, int pred_num);




uint64_t calculate_relations ( Predicates * ); //return how many relations we are going to use



typedef struct {
	int **farrays;
	result_list *r_list;
} Between;

char *check_sums_generate_string( Check_sums * , all_data * , Between * );
//ESSENTIALS

Between *exec_preds ( Predicates * , all_data * , Between * );


//JOINS

Between *execute_join ( Between * , int * , Predicate * , all_data * , int );

relation *prepare_relation ( Between * , all_data * , int  , int , int , int  );

Between *between_update_result_list ( Between * , relation * , int , int , int );

int between_check_if_joined ( Between * , int );

relation *build_relation_from_result_list ( result_list * , all_data * , int , int , int );

int calculate_tuples_from_result_list ( result_list * , all_data * , int , int , int );

//FILTERS

int *execute_filter ( Predicates * , all_data * , Predicate * , int * ) ; //Returns array of the rowids remaining after filter

int *greater_filter ( all_data * , int * , Predicate *);

int *lesser_filter ( all_data * , int * , Predicate *);

int *equal_filter ( all_data * , int * , Predicate *);

int check_if_filtered ( int * , int , int ); //Check if a relation has been filtered

relation *build_relation_from_filtered( int *, all_data * , int , int );// Form a relation from

int calc_tuples_size_to_build_rel_from_filtered( int * , all_data * , int , int ); //calculate number of tuples ne need to malloc in build_relation()

uint64_t between_get_sum ( Between * , all_data * , int , int , int ); 

//OTHERS

int *generate_flags ( int * , int * , int , int , int );

//CONSTRUCTORS

Between *create_between ( Between * , int ); //Allocate memory for our between struct

Between *create_arrays ( Between * , int )  ;

#endif