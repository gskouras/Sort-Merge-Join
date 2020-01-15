#include "../HEADERS/job_list.h"




//JOB LIST FUNCTIONS

job_list *parse_line ( job_list *this , char *line , all_data *datatable ) {
	char rel_str[50]; 
	char pred_str[200];
	char check_sums_str[50];

	int *alias_array = NULL;
	int num_of_rel, num_of_pred, num_of_check_sums;
	Predicates predicates;
  	Check_sums check_sums;

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
	predicates.predicates_array = malloc(sizeof(Predicate) * num_of_pred); 
	predicates.size = num_of_pred;

	fill_predicates(pred_str, &predicates, alias_array);//fill table of predicates

  	num_of_check_sums = find_num_of_relations(check_sums_str);
  	check_sums.check_sums_array = malloc(sizeof(Check_sum) * num_of_check_sums);
  	check_sums.size = num_of_check_sums;

 
  	fill_check_sums(check_sums_str, &check_sums, alias_array);//fill table of check_sums

  	this = job_list_add_job ( this , &predicates , &check_sums , datatable , 0 );

  	return this;
}

job_list *job_list_add_job ( job_list *this , void *arg1 , void *arg2 , all_data *datatable , int type ) {
	//printf("HAHA %d\n", ((Predicates *) arg1)->predicates_array[0].rel1_origin );
	job_node *curr_node = this->root;
	job_node * prev_node = this->root;
	if ( this->root == NULL) { //This means it is a query job
		this->root = job_node_create( this->root , arg1 , arg2 , datatable , type );
	}
	else {
		while ( curr_node != NULL ) {
			prev_node = curr_node;
			curr_node = curr_node->next;
		}
		prev_node->next = job_node_create( prev_node->next , arg1 , arg2 , datatable , type );
	}
	return this;
}

void job_list_print_jobs  (job_list *this ) {
	job_node *curr_node = this->root;
	while ( curr_node != NULL ){
		printf( "ASD\n");
		curr_node = curr_node->next;
	}
}

//CONSTRUCTORS

job_list *job_list_create ( job_list *this ) {
	this = malloc(sizeof ( job_list ) );
	this->root = NULL;
	this->current_jobs = 0;
	return this;
}

job_node *job_node_create ( job_node *this , void *arg1 , void *arg2 , all_data *datatable , int type ) { 
	this = malloc(sizeof(job_node));
	this->next = NULL;
	this->type = type;
	if ( type == 0) {
		this->qjob = query_job_create ( this->qjob , ((Predicates *)arg1) , ((Check_sums *)arg2), datatable );
	}
	return this;
}

query_job *query_job_create ( query_job *this , Predicates *predicates , Check_sums *check_sums , all_data *datatable ) {
	this = malloc(sizeof(query_job));
	this->predicates = predicates;
	this->checksums = check_sums;
	this->datatable = datatable;
	return this;
}