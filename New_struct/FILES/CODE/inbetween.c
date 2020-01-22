#include "../HEADERS/inbetween.h"



Between *exec_preds ( Predicates *pd, all_data *datatable , Between *b ) {


	printf("\n");
	int psize = pd->size;
	int *cur_array;
	int fflag;
	//First calculate how many relations we are going to use
	uint64_t total_rels = calculate_relations ( pd );
	b = create_between ( b , total_rels);

	int filtered[total_rels]; //Keep which rels we filtered
	int filter_count = 0;
	Predicate *temp_pred;
	//First execute filter predicates
	for (int i = 0 ; i < psize; i++)
	{
		temp_pred = &pd->predicates_array[i];
		if ( temp_pred->rel2_alias == -1 )
		{ //This means that we have a filter predicate
			//execute filter
			cur_array = execute_filter( pd , datatable , temp_pred , cur_array );
			b->farrays[temp_pred->rel1_alias] = cur_array;
			//Add to filtered
			fflag = in_used_relation( filtered , filter_count , temp_pred->rel1_alias);
			if ( fflag ) 
			{
				filtered[filter_count++] = temp_pred->rel1_alias;
			}
		}
	}
	int *flags;
	//Now execute the joins
	for ( int i = 0 ; i < psize ; i++ ) {
		temp_pred = &pd->predicates_array[i];
		if ( temp_pred->rel2_alias != -1 ) {
			int rel1_alias = temp_pred->rel1_alias;
			int rel2_alias = temp_pred->rel2_alias;
			flags = generate_flags ( flags , filtered , filter_count , rel1_alias , rel2_alias );
			if ( i > 0 && b->r_list->root != NULL ){
				b = execute_join ( b , flags , temp_pred , datatable , total_rels );
			}
			else if ( i == 0 ) {
				b = execute_join ( b , flags , temp_pred , datatable , total_rels );
			}
		}
	}


	
	//result_list_print_nodes(b->r_list);
	

	free (flags);
	return b;
}


//JOINS

Between *execute_join ( Between *b , int *flags , Predicate *temp_pred , all_data *datatable , int total_rels ) {
	int rel1_alias,rel2_alias,rel1_origin,rel2_origin,col1_no,col2_no;
	rel1_alias = temp_pred->rel1_alias;
	rel2_alias = temp_pred->rel2_alias;
	rel1_origin = temp_pred->rel1_origin;
	rel2_origin = temp_pred->rel2_origin;
	col1_no = temp_pred->rel1_col;
	col2_no = temp_pred->rel2_col;
	relation *result;

	//This function returns a relation built from filtered (or joined) if it was filterered(or joined) or from the datatable if not
	relation *r1 = prepare_relation ( b , datatable , rel1_origin , rel1_alias , col1_no , flags[0] ); 
	relation *r2 = prepare_relation ( b , datatable , rel2_origin , rel2_alias , col2_no , flags[1] );

	if ( rel1_origin != rel2_origin )
	{
		result = join ( r1 , r2 );
	}
	else if ( rel1_origin == rel2_origin)
	{
		printf("SELF JOIN\t");
	}

	b = between_update_result_list ( b , result , rel1_alias , rel2_alias , total_rels );
	
	//free(r1);
	//free(r2);
	//free(result);
	return b;
}

Between *between_update_result_list ( Between *b , relation *result , int rel_left , int rel_right , int total_rels ) {
	int flag;
	if ( b->r_list->root == NULL ) {
		b->r_list = result_list_fill_empty ( b->r_list , result , rel_left , rel_right , total_rels ) ;
	}
	else {
		//First find the reference relation
		int rel_ref;
		int rel_add;
		if ( between_check_if_joined ( b , rel_left ) ) {
			rel_ref = rel_left;
			rel_add = rel_right;
			flag = 1;
			b->r_list = result_list_update (  b->r_list , result , rel_ref , rel_add , flag , total_rels ) ;
		}
		else if ( between_check_if_joined ( b , rel_right ) ) {
			rel_ref = rel_right;
			rel_add = rel_left;
			flag = 0;
			b->r_list = result_list_update (  b->r_list , result , rel_ref , rel_add , flag , total_rels ) ;
		}
		else {
			//BOTH OF THEM ARE ALREADY JOINED
			printf("BOTH ARE JOINED\n");
		}

	}

	return b;
}

relation *prepare_relation ( Between *b , all_data *datatable ,int rel_origin , int rel_alias , int col_no , int flag ) {
	relation *to_return;

	if ( b->r_list->root == NULL ) { //This means this relation hasnt been joined
		if (flag == 1) { //This means that the realtion was filtered
			to_return = build_relation_from_filtered ( b->farrays[rel_alias] , datatable , rel_origin, col_no );
			int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
			bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
			return to_return;
		}
		else { //This means that the relation hasn't been joined nor filtered
			to_return = datatable->table[rel_origin]->columns[col_no];
			int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
			bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
			return to_return;
		}
	}
	else { 
		if ( between_check_if_joined ( b , rel_alias ) ) {
			to_return = build_relation_from_result_list ( b->r_list , datatable , rel_alias , rel_origin , col_no ) ;
			int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
			bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
			return to_return;
		}
		else {
			if (flag == 1) { //This means that the relation was filtered and already joined 
				to_return = build_relation_from_filtered ( b->farrays[rel_alias] , datatable , rel_origin, col_no );
				int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
				bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
				return to_return;
			}
			else { //This means that the relation hasn't been joined or filtered
				to_return = datatable->table[rel_origin]->columns[col_no];
				int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
				bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
				return to_return;
			}
		}
	}
}

int between_check_if_joined ( Between *b , int rel_no ) {
	return result_list_check_if_joined ( b->r_list , rel_no ) ;
}

relation *build_relation_from_result_list ( result_list *r , all_data *datatable , int rel_alias , int rel_origin , int col_no ) {

	int num_of_tuples = r->total_results;
	int final_size = calculate_tuples_from_result_list ( r , datatable , rel_alias , rel_origin , col_no );
	relation *updated_rel = malloc(sizeof(relation));
	updated_rel->tuples = malloc(sizeof(tuple) * final_size);
	updated_rel->num_tuples = final_size;

	int rowid;
	int flag = 1;
	int counter = 0;

	result_node *curr_node = r->root;
	result_node *prev_node;

	while ( curr_node != NULL ) {
		if ( flag ) {
			rowid = result_node_get_rowid ( curr_node , rel_alias ) ;
			updated_rel->tuples[counter].payload = datatable->table[rel_origin]->columns[col_no]->tuples[rowid].payload;//PAYLOAD IS VALUE
			updated_rel->tuples[counter].key = rowid; //KEY IS ROWID
			counter++;
			flag = 0;
		}
		else {
			rowid = result_node_get_rowid ( curr_node , rel_alias ) ;
			if ( rowid != result_node_get_rowid ( prev_node , rel_alias ) ) {
				updated_rel->tuples[counter].payload = datatable->table[rel_origin]->columns[col_no]->tuples[rowid].payload;//PAYLOAD IS VALUE
				updated_rel->tuples[counter].key = rowid; //KEY IS ROWID
				counter++;
			}
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
	return updated_rel;
}

int calculate_tuples_from_result_list ( result_list *r , all_data *datatable , int rel_alias , int rel_origin , int col_no ) {
	int num_of_tuples = r->total_results;
	int counter = 0;
	int sum = 0;
	int rowid;
	int flag = 1;
	result_node *curr_node = r->root;
	result_node *prev_node;
	while ( curr_node != NULL ) {
		if ( flag ) {
			counter++;
			flag = 0;
		}
		else {
			rowid = result_node_get_rowid ( curr_node , rel_alias ) ;
			if ( rowid != result_node_get_rowid ( prev_node , rel_alias ) ) {
				counter++;
			}
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
	return counter;

}

uint64_t between_get_sum ( Between *b , all_data *datatable , int rel_or , int rel_al , int col_no) {
	result_node *curr_node = b->r_list->root;
	int rowid;
	uint64_t sum = 0;
	while( curr_node != NULL ) {
		rowid = curr_node->rels[rel_al];
		sum += datatable->table[rel_or]->columns[col_no]->tuples[rowid].payload;
		curr_node = curr_node->next;
	}
	return sum;
}

//FILTERS


int *execute_filter( Predicates * pd , all_data * dt , Predicate * temp_pred , int *result ) 
{

	int rel_no = temp_pred->rel1_origin;
	int rel_size = dt->table[rel_no]->numTuples;

	//Allocate memory in our result array
	result = malloc ( rel_size * sizeof ( int ) );
	for ( int i = 0 ; i < rel_size ; i++ ) 
	{
		result[i] = i;
	}

	char op = temp_pred->op;
	if ( op == '>' )
	{
		greater_filter ( dt , result , temp_pred );
	}
	else if ( op == '<' ) 
	{
		lesser_filter ( dt , result , temp_pred );
	}
	else if (op == '=' )
	{
		equal_filter ( dt , result , temp_pred );
	}

	return result;
}

int *greater_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{
	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ )
	{
		if ( temp_column->tuples[i].payload <= value ) 
		{
			result[i] = -1;
		}
	}

	return result;
}

int *lesser_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{

	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ ) 
	{
		if ( temp_column->tuples[i].payload > value ) 
		{
			result[i] = -1;
		}
	}

	return result;
}

int *equal_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{

	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ ) 
	{
		if ( temp_column->tuples[i].payload != value ) 
		{
			result[i] = -1;
		}
	}

	return result;
}

int check_if_filtered ( int *filtered , int filter_count , int rel ) 
{ //Return 1 if filtered
	for ( int i = 0 ; i < filter_count ; i++ ) 
	{
		if ( filtered[i] == rel )
		{
			return 1;
		}
	}
	return 0;
}

relation * build_relation_from_filtered(int * array, all_data *dt, int rel_no , int col_no )
{
	uint64_t size = dt->table[rel_no]->numTuples;
	int count= 0;
	
	int num_of_tuples = calc_tuples_size_to_build_rel_from_filtered(array, dt, rel_no , col_no );


	relation *updated_rel = malloc(sizeof(relation));
	updated_rel->tuples = malloc(sizeof(tuple) * num_of_tuples);
	updated_rel->num_tuples = num_of_tuples;
	

	for (int i = 0; i < size ; i++)
	{
		if( array[i] != -1 )
		{	
			updated_rel->tuples[count].key = i;
			updated_rel->tuples[count].payload = dt->table[rel_no]->columns[col_no]->tuples[i].payload;
			count++;

		}
	}
	return updated_rel;
}



int calc_tuples_size_to_build_rel_from_filtered(int *array, all_data *dt, int rel_no , int col_no) 
{
	uint64_t size = dt->table[rel_no]->numTuples;
	int count = 0;

	for (int i = 0; i < size ; ++i)
	{
		if(array[i] != -1)
		{
			count++;
		}
	}
	return count;
}

//OTHERS

int *generate_flags ( int *flags , int *filtered , int filter_count , int rel1 , int rel2 ) {
	flags = malloc ( 2 * sizeof(int) ) ;
	flags[0] = 0;
	flags[1] = 0;
	for ( int i = 0 ; i < filter_count ; i++ ) {
		if ( rel1 == filtered[i] ) {
			flags[0] = 1;
		}
		else if ( rel2 == filtered[i] ) {
			flags[1] = 1;
		}
	}
	return flags;
}

//CONSTRUCTORS


Between *create_between ( Between *this , int total_rels ) //Allocate memory for our between struct
{ 
	this = malloc ( sizeof ( Between ) );
	this = create_arrays ( this , total_rels);
	this->r_list = create_result_list ( this->r_list ) ;
}

Between *create_arrays ( Between *this , int total_rels ) { //Allocate memory for the filtered arrays we are going to create
	this->farrays = malloc ( total_rels * sizeof ( int * ) );
}