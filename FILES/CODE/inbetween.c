#include "../HEADERS/inbetween.h"



//////////////
//ESSENTIALS//
//////////////

void execute_predicates(Predicates *pd, all_data *datatable)
{
	Between *b = create_between ( b ) ;
	int psize = pd->size;
	int temp_rel;
	int *cur_array;

	//First calculate how many relations we are going to use
	int total_arrays = calculate_relations ( pd );
	//Allocate space for our arrays
	b = create_arrays ( b , total_arrays );

	int filtered[total_arrays]; //Keep which rels we filtered
	int filter_count = 0;
	int fflag;

	Predicate *temp_pred;
	//First execute filter predicates
	for (int i = 0 ; i < psize; i++)
	{
		temp_pred = &pd->predicates_array[i];
		if ( temp_pred->rel2_alias == -1 ){ //This means that we have a filter predicate
			//execute filter
			cur_array = execute_filter( pd , datatable , temp_pred , cur_array );
			b->farrays[temp_pred->rel1_alias] = cur_array;
			//Add to filtered
			fflag = in_used_relation( filtered , filter_count , temp_pred->rel1_alias);
			if ( fflag ) {
				filtered[filter_count++] = temp_pred->rel1_alias;
			}
		}
	}

	for (int i = 0 ; i < filter_count ; i++ ) {
		printf("FILTER %d\n", filtered[i]);
	}

	//Then execute joins
	for (int i = 0 ; i < psize; i++)
	{
		temp_pred = &pd->predicates_array[i];
		if ( temp_pred->rel2_alias != -1 ){ //This means that we have a koin predicate
			//First check if rel1 is filtered
			fflag = check_if_filtered ( filtered , filter_count , temp_pred->rel1_alias );
			if ( fflag ) {
				printf("REL no%d is filtered\n", temp_pred->rel1_origin);
			}
			//Then check if rel2 is filtered
			fflag = check_if_filtered ( filtered , filter_count , temp_pred->rel2_alias );
			if ( fflag ) {
				printf("REL no%d is filtered\n", temp_pred->rel2_origin);
			}
		}
	}

}

int *execute_filter( Predicates * pd , all_data * dt , Predicate * temp_pred , int *result ) {

	int rel_no = temp_pred->rel1_origin;
	int rel_size = dt->table[rel_no]->numTuples;

	//Allocate memory in our result array
	result = malloc ( rel_size * sizeof ( int ) );
	for ( int i = 0 ; i < rel_size ; i++ ) {
		result[i] = i;
	}

	char op = temp_pred->op;
	if ( op == '>' ){
		greater_filter ( dt , result , temp_pred );
	}
	else if ( op == '<' ) {
		lesser_filter ( dt , result , temp_pred );
	}
	else {
		equal_filter ( dt , result , temp_pred );
	}

	//for (int i = 0; i < 100 ; i++ ) {
	//	printf("%d\n", result[i]);
	//}
	return result;
}

int check_if_filtered ( int *filtered , int filter_count , int rel ) { //Return 1 if filtered
	for ( int i = 0 ; i < filter_count ; i++ ) {
		if ( filtered[i] == rel ){
			return 1;
		}
	}
	return 0;
}

///////////
//FILTERS//
///////////

int *greater_filter ( all_data *dt , int *result , Predicate *temp_pred ) {

	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ ) {
		if ( temp_column->tuples[i].payload <= value ) {
			result[i] = -1;
		}
	}

	return result;
}

int *lesser_filter ( all_data *dt , int *result , Predicate *temp_pred ) {

	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ ) {
		if ( temp_column->tuples[i].payload > value ) {
			result[i] = -1;
		}
	}

	return result;
}

int *equal_filter ( all_data *dt , int *result , Predicate *temp_pred ) {

	int rel_no = temp_pred->rel1_origin;
	int col_no = temp_pred->rel1_col;
	uint64_t value =  temp_pred->filter_value;

	uint64_t size = dt->table[rel_no]->numTuples;
	relation *temp_column = dt->table[rel_no]->columns[col_no];


	for ( int i = 0 ; i < size ; i++ ) {
		if ( temp_column->tuples[i].payload != value ) {
			result[i] = -1;
		}
	}

	return result;
}

////////////////
//CONSTRUCTORS//
////////////////



Between *create_between ( Between *this ) { //Allocate memory for our between struct
	this = malloc ( sizeof ( Between ) );
}


Between *create_arrays ( Between *this , int total_arrays ) {  //Allocate memory for the filtered arrays we are going to create

	this->farrays = malloc ( total_arrays * sizeof ( int * ) );
	this->jarrays = malloc ( total_arrays * sizeof ( int * ) );

}


//////////
//OTHERS//
//////////


int calculate_relations ( Predicates *pd ) {

	int used[pd->size];
	int count = 0 ;
	int rel1,rel2; //Here we store the rel origin
	int flag ;

	for (int i = 0; i < pd->size ; i++ ) {
		rel1 = pd->predicates_array[i].rel1_alias;
		flag = in_used_relation ( used , count , rel1 );  //When flag is 1 that means rel is not already use so we add it to used array
		if ( flag ) {
			used[count] = rel1;
			count++;
		}
		rel2 = pd->predicates_array[i].rel2_alias;
		if ( rel2 != -1 ) {
			flag = in_used_relation ( used , count , rel2);
			if ( flag ) {
				used[count] = rel2;
				count++;
			}
		}
	}

	return count;
}

int in_used_relation ( int *array , int count , int rel_no ) {

	for (int i = 0; i < count + 1; i++ ) {
		if ( rel_no == array[i] ) {
			return 0;
		}
	}
	return 1;
}


