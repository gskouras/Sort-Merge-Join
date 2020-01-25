#include "../HEADERS/job_list.h"




//JOB LIST FUNCTIONS

job_list *parse_line ( job_list *this , char *line , all_data *datatable , int id ) {
	char rel_str[50]; 
	char pred_str[200];
	char check_sums_str[50];

	int *alias_array = NULL;
	int num_of_rel, num_of_pred, num_of_check_sums;
	Predicates *predicates;
  	Check_sums *check_sums;
  	predicates = malloc ( sizeof(Predicates));
  	check_sums = malloc ( sizeof(Check_sums));

	char *token;
	token = strtok(line, "|");
	strcpy(rel_str, token);

	token = strtok(NULL, "|");
	strcpy(pred_str, token);

	token = strtok(NULL, "\n");
	strcpy(check_sums_str, token);

	num_of_rel = find_num_of_relations(rel_str); //find number of relations for current query;
	alias_array = (int *)malloc(sizeof(int) * num_of_rel); //give space to save the relations

  	fill_relations(rel_str, alias_array); //fill array of relations

	num_of_pred = find_num_of_predicates(pred_str); //calculating number of predicates for current query
	predicates->predicates_array = malloc(sizeof(Predicate) * num_of_pred); 
	predicates->size = num_of_pred;

	fill_predicates(pred_str, predicates , alias_array);//fill table of predicates

  	num_of_check_sums = find_num_of_relations(check_sums_str);
  	check_sums->check_sums_array = malloc(sizeof(Check_sum) * num_of_check_sums);
  	check_sums->size = num_of_check_sums;

 
  	fill_check_sums(check_sums_str, check_sums, alias_array);//fill table of check_sums

  	this = job_list_add_job ( this , predicates , check_sums , datatable , 0 , id);

  	return this;
}

job_list *job_list_add_job ( job_list *this , void *arg1 , void *arg2 , all_data *datatable , int type , int id) {
	job_node *curr_node = this->root;
	job_node * prev_node = this->root;
	if ( this->root == NULL) { //This means it is a query job
		this->root = job_node_create( this->root , arg1 , arg2 , datatable , type , id );

		this->current_jobs++;
	}
	else {
		while ( curr_node != NULL ) {
			prev_node = curr_node;
			curr_node = curr_node->next;
		}
		prev_node->next = job_node_create( prev_node->next , arg1 , arg2 , datatable , type , id );
		this->current_jobs++;
	}
	



	return this;
}

job_node *job_list_get_job ( job_list *this ) {
	int flag = 1;
	job_node *curr_node = this->root;
	while ( curr_node != NULL ) {
		if ( curr_node->used == 0 ) {
			curr_node->used = 1;
			flag = 0;
			return curr_node;
		}
		curr_node = curr_node->next;
	}
	if (flag) {
		return NULL;
	}
}

void job_list_remove_job ( job_list *this , int id , int type ) {
	int counter = 0;

	if ( type == 0 ) {
		job_node *curr_node = this->root;
		job_node *prev_node = this->root;
		while ( curr_node->qjob->id!=id ) {
			prev_node = curr_node;
			curr_node = curr_node->next;
			counter++;
		}
		if ( counter == 0 ){ //THIS IS THE ROOT
			this->root = curr_node->next;
			this->current_jobs--;
		}
		else {
			prev_node->next = curr_node->next;
			this->current_jobs--;
		}
	}
	if ( this->current_jobs == 0 ) {
		this->empty = 1;
	}
}



void job_list_print_jobs  (job_list *this ) {
	job_node *curr_node = this->root;
	while ( curr_node != NULL ){
		printf("NODE %d \n", curr_node->qjob->predicates->predicates_array[0].rel1_origin);
		curr_node = curr_node->next;
	}
	printf("\n\n");
}




//CONSTRUCTORS

job_list *job_list_create ( job_list *this ) {
	this = malloc(sizeof ( job_list ) );
	this->root = NULL;
	this->current_jobs = 0;
	this->empty = 0;
	return this;
}

job_node *job_node_create ( job_node *this , void *arg1 , void *arg2 , all_data *datatable , int type , int id ) { 
	this = malloc(sizeof(job_node));
	this->next = NULL;
	this->type = type;
	this->used = 0;
	if ( type == 0) {
		this->qjob = query_job_create ( this->qjob , ((Predicates *)arg1) , ((Check_sums *)arg2), datatable , id );
	}
	return this;
}

query_job *query_job_create ( query_job *this , Predicates *predicates , Check_sums *check_sums , all_data *datatable , int id ) {


	this = malloc(sizeof(query_job));
	this->predicates = predicates;
	this->checksums = check_sums;
	this->datatable = datatable;
	this->id = id;
	return this;
}