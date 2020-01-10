#include "../HEADERS/result_list.h"

void check ( void );


void check ( void ) {


	relation *r1 = malloc ( sizeof ( relation ) ) ;
	r1->tuples = malloc ( 6 * sizeof (tuple));
	r1->num_tuples = 6;

	relation *r2 = malloc ( sizeof ( relation ) ) ;
	r2->tuples = malloc ( 3 * sizeof (tuple));
	r2->num_tuples = 3;

	r1->tuples[0].key = 1;
	r1->tuples[0].payload = 11;
	r1->tuples[1].key = 2;
	r1->tuples[1].payload = 11;
	r1->tuples[2].key = 3;
	r1->tuples[2].payload = 11;
	r1->tuples[3].key = 4;
	r1->tuples[3].payload = 8;
	r1->tuples[4].key = 5;
	r1->tuples[4].payload = 10;
	r1->tuples[5].key = 6;
	r1->tuples[5].payload = 10;


	r2->tuples[0].key = 10;
	r2->tuples[0].payload = 11;
	r2->tuples[1].key = 11;
	r2->tuples[1].payload = 20;
	r2->tuples[2].key = 12;
	r2->tuples[2].payload = 13;

	relation *j = join ( r1 , r2 );

	relation_print ( j );
}