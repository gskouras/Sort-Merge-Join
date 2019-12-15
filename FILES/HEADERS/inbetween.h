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
} Between;

//////////////
//ESSENTIALS//
//////////////

void execute_predicates(Predicates *pd, all_data *dt);

int *execute_filter( Predicates * , all_data * , Predicate * , int * ) ; //Returns array of the rowids remaining after filter

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

int calculate_relations ( Predicates * ); //return how many relations we are going to use

int in_used_relation ( int * , int , int ); //Check if already used




#endif