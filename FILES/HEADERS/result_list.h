#ifndef _RESULTLIST_H_
#define _RESULTLIST_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/sort_join.h"

//STRUCTS

typedef struct Result_node {
	int *rels;
	struct Result_node *next;
	int total_rels;
} result_node ;


typedef struct {
	result_node *root;
	int total_results;
} result_list ;

//ESSENTIALS


//OTHERS

int in_used_relation ( int * , int , int ); //Check if already used

//RESULT LIST FUNCTIONS

result_list *result_list_update (  result_list * , relation * , int , int , int , int ) ;

result_list *result_list_fill_empty ( result_list * , relation * , int  , int , int );

result_list *result_list_add_node ( int , result_list * );

void result_list_delete_node ( result_list * , int ) ;

int result_list_count_nodes ( result_list * );

void result_list_print_nodes ( result_list * );

void result_list_set_rowid ( result_list * , int , int , int );

int result_list_get_rowid ( result_list * , int , int );

//RESULT NODE FUNCTIONS

result_node *result_node_clone ( result_node * , int );

void result_node_delete_node ( result_node * ) ;

void result_node_set_rowid ( result_node * , int , int );

int result_node_get_rowid ( result_node * , int );

int result_list_check_if_joined ( result_list * , int );

//CONSTRUCTORS

result_list *create_result_list ( result_list * ) ;

result_node *create_result_node ( result_node * , int ) ;

#endif