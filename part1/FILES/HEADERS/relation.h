#include "../HEADERS/tuple.h"


typedef struct relation {
	tuple *tuples;
	int num_tuples;
} relation ;

relation * relation_create ( relation * /*relation to be created*/ ) ;

relation * relation_createtuples ( relation * , int /*tota tuples of relation*/) ;



void relation_setkey ( relation * , int  /*position of tuple that we want to set*/ , unsigned long long /*key we want to set*/ ) ;

unsigned long long relation_getkey ( relation * , int /*position of tuple that we want to get*/) ;

void relation_setpayload ( relation * , int /*position of tuple that we want to set*/ , unsigned long long /*payload we want to set*/ ) ;

unsigned long long relation_getpayload ( relation * , int tup /*position of tuple that we want to get*/) ;

int relation_getnumtuples ( relation * ) ;

int relation_checkifsorted ( relation *  ) ;

void relation_print ( relation * ) ;

void relation_free ( relation * ) ;

