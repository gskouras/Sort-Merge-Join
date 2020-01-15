#ifndef _JOBLIST_H_
#define _JOBLIST_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include <pthread.h>

#include "../HEADERS/sort_join.h"
#include "../HEADERS/query.h"
#include "../HEADERS/inbetween.h"


//STRUCTS 

/////////////////////////////
//PARALLALALA ON QUERY LEVEL/
/////////////////////////////

typedef struct {
	Predicates *predicates;
	Check_sums *checksums;
	all_data *datatable;
} query_job;


typedef struct Job_node{
	struct Job_node *next;
	int type; //IF 0 THEN QJOB ELSE IF 1 THEN JJOB ELSE IF 2 SJOB
	query_job *qjob;
	//join_job *jjob;
	//sort_job *sjob;
} job_node;

typedef struct {
	job_node *root;
	int current_jobs;
} job_list;


//JOB LIST FUNCTIONS

job_list *job_list_add_job ( job_list * , void * , void *, all_data * , int );

job_list *parse_line ( job_list * , char * , all_data *);

void job_list_print_jobs  ( job_list * );
//CONSTRUCTOS

job_list *job_list_create ( job_list * );

job_node *job_node_create ( job_node * , void * , void * , all_data * , int );

query_job *query_job_create ( query_job * , Predicates * , Check_sums * , all_data * );




#endif


///////////////////////////////////
//PARALALALA ON JOIN OR SORT LEVEL/
///////////////////////////////////

//QUERY JOB
/*
typedef struct {
	Check_sums *checksums;
	Between *b;
	all_data *datatable;
} query_job;
*/
// JOIN JOB
/*
typedef {
	Predicate *predicate;
	Between *b;
} join_job;
*/

//SORT JOB
/*
typedef {
	relation *r;
	//RETURNS THE SORTED RELATION
}
*/