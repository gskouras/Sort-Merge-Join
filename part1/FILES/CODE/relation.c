#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../HEADERS/relation.h"


relation * relation_create ( relation *this ) {
	this = malloc ( sizeof ( relation ) ) ;
	return this;
}

relation * relation_createtuples ( relation *this , int num /* number of tuples */ ) {
	//
	this->num_tuples = num ;
	//
	this->tuples = malloc ( num * sizeof(tuple) ) ;

	return this;
}


void relation_setkey( relation *this , int tup /*position of tuple that we want to set*/ , unsigned long long key /*key we want to set*/ ) {
	
	tuple_setkey ( &this->tuples[tup] , key );

}

unsigned long long relation_getkey ( relation *this , int tup /*position of tuple that we want to get*/) {
	
	unsigned long long key = tuple_getkey ( &this->tuples[tup] );
	return key;

}




void relation_setpayload ( relation *this , int tup /*position of tuple that we want to set*/ , unsigned long long payl /*payload we want to set*/ ) {
	
	tuple_setpayload ( &this->tuples[tup] , payl );

}

unsigned long long relation_getpayload ( relation *this , int tup /*position of tuple that we want to get*/) {
	
	unsigned long long payl = tuple_getpayload ( &this->tuples[tup] );
	return payl;

}

int relation_getnumtuples ( relation * this ) {
	return this->num_tuples ;
}

int relation_checkifsorted ( relation * this ) {
	int flag = 0;
	int total_tuples = relation_getnumtuples ( this ) ;
	for (int i = 1; i <=total_tuples ; i++ ){
		if ( relation_getkey(this,i) <= relation_getkey(this,i-1)) {
			//printf("%llu is smaller than %llu \n",relation_getkey(this,i) , relation_getkey(this,i-1) );
			flag = 1;
		}
	}
	return flag ;
}

void relation_print ( relation *rel ) {
	int total_tuples = relation_getnumtuples ( rel ) ;
	for (int i = 0; i < total_tuples ; i++ ) {
		printf ( "KEY IS %llu PAYLOAD IS %llu \n" , relation_getkey ( rel , i ) , relation_getpayload ( rel , i ) ) ;
	}
}

void relation_free ( relation *rel ) {
	free (rel->tuples);
}