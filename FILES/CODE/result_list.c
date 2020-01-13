
#include "../HEADERS/result_list.h"


//ESSENTIALS

//OTHERS



int calculate_relations ( Predicates *pd ) 
{
	int used[pd->size];
	int count = 0 ;
	int rel1,rel2; //Here we store the rel origin
	int flag ;

	for (int i = 0; i < pd->size ; i++ ) 
	{
		rel1 = pd->predicates_array[i].rel1_alias;
		flag = in_used_relation ( used , count , rel1 );  //When flag is 1 that means rel is not already use so we add it to used array
		if ( flag ) 
		{
			used[count] = rel1;
			count++;
		}
		rel2 = pd->predicates_array[i].rel2_alias;
		if ( rel2 != -1 ) 
		{
			flag = in_used_relation ( used , count , rel2);
			if ( flag ) 
			{
				used[count] = rel2;
				count++;
			}
		}
	}

	return count;
}

int in_used_relation ( int *array , int count , int rel_no ) {

	for (int i = 0; i < count + 1; i++ ) 
	{
		if ( rel_no == array[i] ) 
		{
			return 0;
		}
	}
	return 1;
}


//RESULT LIST FUNCTIONS


result_list *result_list_update (  result_list * r, relation * result , int rel_ref , int rel_add , int flag , int total_rels )
{
	int total_results = relation_getnumtuples(result);
	int found = 0;
	int counter = 0;
	int pos = 0;
	int ref_value , add_value , compare_value , next_cmp_value;
	result_node *add_node ;

	result_node *curr_node = r->root;
	result_node *next_node;

	while ( curr_node != NULL ) 
	{
		next_node = curr_node->next;
		ref_value = result_node_get_rowid ( curr_node , rel_ref );
		for ( int i = 0 ; i < total_results ; i++ ) 
		{

			if ( flag ) 
			{
				add_value = result->tuples[i].key;
				compare_value = result->tuples[i].payload;
				next_cmp_value = result->tuples[i+1].payload;
			}
			else 
			{
				add_value = result->tuples[i].payload;
				compare_value = result->tuples[i].key;
				next_cmp_value = result->tuples[i+1].key;
			}

			//printf("%ld %ld \n", result->tuples[i].payload , result->tuples[i].key);

			if ( ref_value == compare_value ) 
			{
				if ( counter == 0 ) 
				{
					result_node_set_rowid ( curr_node , rel_add , add_value );
					counter++;
				}
				else if(ref_value == compare_value  && ref_value == next_cmp_value)
				{
					add_node = result_node_clone ( curr_node , total_rels );
					r->total_results++;
					result_node_set_rowid ( add_node , rel_add , add_value );
					add_node->next = curr_node->next;
					curr_node->next = add_node;
					curr_node = add_node;
					counter++;
				}
				found = 1;
			}

		}

		if ( !found ) 
		{
			result_list_delete_node(r, pos);
		}

		curr_node = curr_node->next;
		counter = 0;
		found = 0;
		pos++;
	}
	//result_list_print_nodes(r);
	return r;
}



result_list *result_list_fill_empty ( result_list *r , relation *result , int rel_left , int rel_right  , int total_rels ) {
	int total_results = relation_getnumtuples(result) ;
	for ( int i = 0 ; i < total_results ; i++ ) {
		r = result_list_add_node ( total_rels , r );
		int node_pos = r->total_results;
		result_list_set_rowid ( r , node_pos-1, rel_left , result->tuples[i].payload );
		result_list_set_rowid ( r , node_pos-1, rel_right , result->tuples[i].key );
	}
	//result_list_print_nodes(r);
	return r;
}

result_list *result_list_add_node ( int total_rels , result_list *r ) {
	result_node *curr_node = r->root;
	result_node *prev_node = r->root;

	if ( r->root == NULL ) {
		r->root = create_result_node ( r->root , total_rels);
		r->total_results++;
	}
	else {
		while ( curr_node != NULL ) {
			prev_node = curr_node;
			curr_node = curr_node->next;
		}
		prev_node->next = create_result_node ( prev_node->next , total_rels ); 
		r->total_results++;
	}

	return r;
}

void result_list_delete_node ( result_list *r , int node_pos ) {
	result_node *curr_node = r->root;
	result_node *prev_node = r->root;
	result_node *next_node;
	int counter = 0;

	if ( node_pos == 0 ) {  //This means tha we want to delete the root node
		curr_node = r->root;
		r->root = r->root->next;
		result_node_delete_node ( curr_node );
		free(curr_node);
		r->total_results--;
	}
	else {
		while ( counter != node_pos ) {
			counter++;
			prev_node = curr_node;
			curr_node = curr_node->next;
		}	
		next_node = curr_node->next;
		prev_node->next = next_node;
		result_node_delete_node ( curr_node );
		free(curr_node);
		r->total_results--;
	}
}

int result_list_count_nodes ( result_list *r ){
	result_node *curr_node = r->root;
	int total = 0;
	while ( curr_node != NULL ) { //Find the end of the list
		total++;
		curr_node = curr_node->next;
	}
	return total;
}

void result_list_print_nodes ( result_list *r ) {
	result_node *curr_node = r->root;
	while ( curr_node != NULL ) {
		for ( int i = 0 ; i < curr_node->total_rels ; i++ ) {
			printf("REL%d rid %d ", i , curr_node->rels[i] );
		}
		printf("\n");
		curr_node = curr_node->next;
	}
}

void result_list_set_rowid ( result_list *r , int node_pos , int rel , int value ) {
	result_node *curr_node = r->root;
	int counter = 0;
	while ( counter != node_pos ) {
		counter++;
		curr_node = curr_node->next;
	}
	result_node_set_rowid ( curr_node , rel , value );
}

int result_list_get_rowid ( result_list *r , int node_pos , int rel ) {
	result_node *curr_node = r->root;
	int counter = 0;
	while ( counter != node_pos ) {
		counter++;
		curr_node = curr_node->next;
	}
	int result = result_node_get_rowid ( curr_node , rel );
	return result;
}

int result_list_check_if_joined ( result_list *r , int rel ) {
	if (r->root == NULL ) {
		return 0;
	}
	else {
		if ( result_list_get_rowid ( r , 0 , rel ) == -1 ) {
			return 0;
		}
		else {
			return 1;
		}
	}
}

//RESULT NODE FUNCTIONS

result_node *result_node_clone ( result_node *curr_node , int total_rels ) {
	result_node *temp_node = create_result_node ( temp_node , total_rels);
	for (int i = 0; i < total_rels; i++){
		temp_node->rels[i] = curr_node->rels[i];
	}
	return temp_node;

}

void result_node_delete_node ( result_node *curr_node ) {
	free(curr_node->rels);
}

void result_node_set_rowid ( result_node *curr_node , int rel , int value ){
	curr_node->rels[rel] = value;
}

int result_node_get_rowid ( result_node *curr_node , int rel ){
	int result = curr_node->rels[rel];
	return result;
}

//CONSTRUCTORS

result_list *create_result_list ( result_list *this_list ) {
	this_list = malloc ( sizeof ( result_list ) ) ;
	this_list->root = NULL;
	this_list->total_results = 0;
	return this_list;
}

result_node *create_result_node ( result_node *this_node , int total_rels) {
	this_node = malloc ( sizeof ( result_node ) ) ;
	this_node->next = NULL;
	this_node->rels = malloc ( total_rels * sizeof(int) ) ;
	this_node->total_rels = total_rels;
	for ( int i = 0 ; i < total_rels ; i++ ) {
		this_node->rels[i] = -1;
	}
	return this_node;
}