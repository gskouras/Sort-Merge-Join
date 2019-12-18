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

///////////
//STRUCTS//
///////////

typedef struct {
	int **farrays;
	int **jarrays;
	int *jarrays_size;
} Between;

//////////////
//ESSENTIALS//
//////////////

Between *execute_predicates(Predicates * , all_data * , Between * );

Between *execute_join ( Predicates * , all_data * , Predicate * , Between *, int * , int * , int , int ) ;//Returns the jarrays updated after the join

int **update_joined ( int ** , int * , int , int , Predicate * , relation * , int * , int , int );

int *execute_filter ( Predicates * , all_data * , Predicate * , int * ) ; //Returns array of the rowids remaining after filter

int check_if_filtered ( int * , int , int );

///////////
//FILTERS//
///////////

int *greater_filter ( all_data * , int * , Predicate *);

int *lesser_filter ( all_data * , int * , Predicate *);

int *equal_filter ( all_data * , int * , Predicate *);

////////////////
//CONSTRUCTORS//
////////////////

Between *create_between ( Between *); //Allocate memory for our between struct

Between *create_arrays ( Between * , int ) ; //Allocate memory for the filtered and the joined arrays we are going to create


//////////
//OTHERS//
//////////

int **fill_new_joined ( int ** , int ** , int  , int , int , int , int , relation * , int );

int **fill_already_joined ( int ** , int ** , int , int i , int , int *, int); 

int calculate_new_size ( int * , int , relation * , int );

int times_in_result ( int , relation * , int ) ;

int calculate_relations ( Predicates * ); //return how many relations we are going to use

int in_used_relation ( int * , int , int ); //Check if already used

int *check_if_used ( int * , int * , int * , int , int , Predicate * );

int check_if_filtered ( int * , int , int ); //Check if a relation has been filtered

int check_if_joined ( int * , int , int ); //Check if a relation has been joined

relation *build_relation_from_filtered( int *, all_data * , int , int );// Form a relation from

int calc_tuples_size_to_build_rel_from_filtered( int * , all_data * , int , int ); //calculate number of tuples ne need to malloc in build_relation()


relation *build_relation_from_joined(int * , int *, all_data * , int , int , int  );

int is_visited( int * , int  , int  );

int calc_tuples_size_to_build_rel_from_joined(int *, int * , all_data * , int , int , int );


#endif

