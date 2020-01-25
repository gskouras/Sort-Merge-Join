#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "../HEADERS/main.h"



int main()
{


	List list;
	relation *R;
	relation *S;

	initlist(&list, MAXSIZE, sizeof(tuple));


	R = relation_create( R ) ;
	S = relation_create( S ) ;


	read_file ( R , "/mnt/c/users/Γιώργος/Desktop/An.Log.Pl.Syst.-Project-1/FILES/Datasets/tiny/relA" ) ;
	read_file ( S , "/mnt/c/users/Γιώργος/Desktop/An.Log.Pl.Syst.-Project-1/FILES/Datasets/tiny/relB" ) ;

	// read_file ( R , "/home/parallels/Desktop/An.Log.Pl.Syst.-Project-1/FILES/Datasets/tiny/relA" ) ;
	// read_file ( S , "/home/parallels/Desktop/An.Log.Pl.Syst.-Project-1/FILES/Datasets/tiny/relB" ) ;


	int r = relation_getnumtuples( R ); //total tuples in r relation
	printf("Total tuples of R relation are %d\n" , R->num_tuples );
	
	int s = relation_getnumtuples( S ); //total tuples in s relation
	printf("Total tuples of S relation are %d\n\n" , S->num_tuples );
	

	bucket_sort ( R , 0 , r-1 , 1 ) ; //FIRST WE SORT THE R RELATIOM
	bucket_sort ( S , 0 , s-1 , 1 ) ; //THEN WE SORT THE S RELATION


	int sortedr = relation_checkifsorted ( R ) ;
	int sorteds =  relation_checkifsorted ( S ) ;

	if (sortedr && sorteds) //chech if relations are indeed sorted
	{
		printf("The relations sorted Succesfully\n\n");
	}else{
		printf("Relations failed to be sorted Succesfully\n\n");
	}

	join(R, S, &list); //Join common keys from Relation R to Relation S.
	
	//Software Testing

	RunAllTest();
	printf("For Software Test Results Check UnityTest.txt!\n\n");

	//Free Memory
	relation_free ( R ) ;
	relation_free ( S ) ;
	free ( R ) ;
	free ( S ) ;
}

void read_file ( relation * rel , char * file_name ) {


	//OPEN THE FILE 

	FILE *fp;
	fp = fopen ( file_name , "r");	

	if ( fp == NULL ) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}


	//FIRST WE COUNT THE TUPLES THAT WELL NEED

	char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    int total_tuples = 0 ; //TOTAL TUPLES



	while ( ( nread = getline( &line , &len , fp ) ) != -1 ) {
			total_tuples++;
	}

	//printf("total tuples are %d\n" , total_tuples );

	free(line);

	fclose(fp);

	//CREATE THE RELATION WITH TOTAL TUPLES

	rel = relation_createtuples ( rel , total_tuples ) ;


	//THEN WE START FILLING THE RELATION

	fp = fopen ( file_name , "r");	

	if ( fp == NULL ) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	line = NULL;
    len = 0;
    nread;
    int tup_no = 0;


	while ( ( nread = getline( &line , &len , fp ) ) != -1 ) {
			extract_line ( rel , line , tup_no ) ;
			tup_no++;
	}


	free(line);

	//CLOSE THE FILE

	fclose(fp);

}


void extract_line ( relation * rel , char * line , int tup_num ) {
	

	//printf ("STRING IS %s\n",line);

	char * key_string;
	char * payload_string;

	//FIRST EXTRACT THE KEY
	key_string = strtok ( line , " ," ) ;

	//SET THE KEY TO TUPLE NUMBER tup_num
	unsigned long long key = strtoll(key_string, NULL, 10);
	relation_setkey( rel , tup_num , key ) ;

	//THEN EXTRACT THE PAYLOAD
	payload_string = strtok ( NULL , " ,\n" ) ;

	//SET THE PAYLOAD
	unsigned long long payload = strtoll ( payload_string, NULL, 10 );
	relation_setpayload ( rel , tup_num , payload ) ;

}
