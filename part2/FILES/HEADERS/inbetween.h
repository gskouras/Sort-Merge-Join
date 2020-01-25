#ifndef _INBETWEEN_H_
#define _INBETWEEN_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/query.h"
#include "../HEADERS/sort_join.h"
#include "../HEADERS/result_list.h"


typedef struct {
	int **farrays;
	result_list *r_list;
} Between;


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