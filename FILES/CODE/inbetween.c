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
	int fflag;
	//First calculate how many relations we are going to use
	int total_arrays = calculate_relations ( pd );
	//Allocate space for our arrays
	b = create_arrays ( b , total_arrays );

	int filtered[total_arrays]; //Keep which rels we filtered
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

	int *flags = malloc ( 2 * sizeof ( int ) ) ; //In this array we keep flags 
	int joined[total_arrays]; //Keep track of relations tha have been used in a join
	int join_count = 0;
	int not_used;

	//Then execute joins
	for (int i = 0 ; i < 2; i++) {
		temp_pred = &pd->predicates_array[i]; 

		if ( temp_pred->rel2_alias != -1 ) {//This means that we have a join predicate 
			flags = check_if_used ( flags , joined , filtered , join_count , filter_count , temp_pred ) ;
			//execute_join

			b = execute_join ( pd , datatable , temp_pred , b , flags );

			//update already joined 

			//Add to joined
			if ( flags[0] == 0 || flags[0] == 1 ) { //This means that rel1 of temp_pred got joined so we add it to joined
				joined[join_count++] = temp_pred->rel1_alias;
			}
			if ( flags[1] == 0 || flags[1] == 1 ) { //This means that rel2 of temp_pred got joined so we add it to joined
				joined[join_count++] = temp_pred->rel2_alias;
			}
		}
	}

	free ( flags );

}

Between *execute_join ( Predicates *pd , all_data *dt , Predicate *temp_pred , Between *b , int *flags ) { //Returns the jarrays updated after the join

	int rel1_no = temp_pred->rel1_origin;
	int rel2_no = temp_pred->rel1_origin;

	relation *cur1_rel;
	relation *cur2_rel;
	relation *result; //Store the result of the jkoin of each case

	if ( (flags[0] == 0 || flags[0] == 1) &&  (flags[1] == 0 || flags[1] == 1) ) { //This means that none of the relations has been joined
		if ( flags[0] == 0 && flags[1] == 0 ) { // CASE 1 : This means that none of the relations has been filtered;
			//SORTJOIN//
			//EXMP result = join(rel1 ,rel2 )
			//FREE//
		}
		else if ( flags[0] == 1 ) { // CASE 2 : This means that the left rel was filtered
			//BUILD RELATION FOR LEFT REL//
			//EXMP cur1_rel = build_Relation ( farrays[rel1_no] );
			//EXMP result = join ( cur1_rel , rel2 );
			//SORTJOIN//
			//FREE//
		}
		else if ( flags[1] == 1 ) { // CASE 3 : This means that the right rel was filtered
			//BUILD RELATION FOR RIGHT REL//
			//SORTJOIN//
			//FREE//
		}
		else { // CASE 4 : This means that both rels are filtered
			//BUILD RELATION FOR LEFT REL//
			//BUILD RELATION FOR RIGHT REL//
			//SORTJOIN//
			//FREE//
		}
	}
	else { //This means that one of the relations has been joined
		if ( flags[0] == 2 && flags[1] != 2) { // If left rel is arledy joined and right isnt
			if (flags[1] == 0 ) { // CASE 5 : This means that right isnt filetered or joined
				//BUILD RELATION FOR LEFT REL//
				//SORT JOIN//
				//FREE//
			}
			else { // CASE 6 : This means that right was filterd but not joined yet
				//BUILD RELATION FOR LEFT REL FROM JARRAYS//
				//BUILD RELATION FOR RIGHT REL FROM FARRAYS//
				//FREE//
			}
		}
		else if ( flags[1] == 2 && flags[0] != 2 ) { // Esle If right rel is arledy joined and left isnt
			if (flags[0] == 0 ) { // CASE 7 : This means that left isnt filetered or joined
				//BUILD RELATION FOR RIGHT REL//
				//SORT JOIN//
				//FREE//
			}
			else { // CASE 8 : This means that left was filterd but not joined yet
				//BUILD RELATION FOR RIGHT REL FROM JARRAYS//
				//BUILD RELATION FOR LEFT REL FROM FARRAYS//
				//FREE//
			}
		}
		else{ //CASE 9 : This means both HAve already joined
			//FILTER CASE//
			//FREE//
		}
	}


	return this;
}

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
	else 
	{
		equal_filter ( dt , result , temp_pred );
	}

	return result;
}

int *check_if_used ( int *flags , int *joined , int *filtered , int join_count , int filter_count , Predicate *temp_pred ) {

	int rel1_no = temp_pred->rel1_alias;
	int rel2_no = temp_pred->rel2_alias;
	//For each relation of the join predicate check if filtered or joined
	//First check if joined;
	flags[0] = check_if_joined ( joined , join_count , rel1_no);
	flags[1] = check_if_joined ( joined , join_count , rel2_no);
	//Then check for the ones that arent already joined if they are filtered
	if ( flags[0] !=2 ) { //We  dont check if filtered if it is joined before
		flags[0] = check_if_filtered ( filtered , filter_count , rel1_no ); //returns 1 if filtered
	}
	if ( flags[1] !=2 ) {
		flags[1] = check_if_filtered ( filtered , filter_count , rel2_no );
	}
	return flags;
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

int check_if_joined ( int *joined , int join_count , int rel ) 
{ //Return 2 if joined
	for ( int i = 0 ; i < join_count  ; i++ ) 
	{
		if ( joined[i] == rel )
		{
			return 2;
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

////////////////
//CONSTRUCTORS//
////////////////



Between *create_between ( Between *this ) //Allocate memory for our between struct
{ 
	this = malloc ( sizeof ( Between ) );
}


Between *create_arrays ( Between *this , int total_arrays )  //Allocate memory for the filtered arrays we are going to create
 { 

	this->farrays = malloc ( total_arrays * sizeof ( int * ) );
	this->jarrays = malloc ( total_arrays * sizeof ( int * ) );

}


//////////
//OTHERS//
//////////


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


relation * build_relation(int * farray, all_data *dt, int rel_no , int col_no )
{
	uint64_t size = dt->table[rel_no]->numTuples;
	int count= 0;
	int num_of_tuples = calc_tuples_size_to_build_rel(farray, dt, rel_no , col_no );

	relation *updated_rel = malloc(sizeof(relation));
	updated_rel->tuples = malloc(sizeof(tuple) * num_of_tuples);
	updated_rel->num_tuples = num_of_tuples;

	for (int i = 0; i < size ; i++)
	{
		if( farray[i] != -1)
		{	
			//printf("i is %d\n", i);
			updated_rel->tuples[count].key = farray[i];
			updated_rel->tuples[count].payload = dt->table[rel_no]->columns[col_no]->tuples[i].payload;
			count++;
		}
	}
	return updated_rel;
}
	


int calc_tuples_size_to_build_rel(int * farray, all_data *dt, int rel_no , int col_no) 
{
	uint64_t size = dt->table[rel_no]->numTuples;
	int count = 0;
	// printf("size is %ld \n", size);
	// printf("The relation has been filtered is r%d col%d\n",rel_no, col_no );

	for (int i = 0; i < size ; ++i)
	{
		if(farray[i] != -1)
		{
			count++;
		}
	}
	return count;
}