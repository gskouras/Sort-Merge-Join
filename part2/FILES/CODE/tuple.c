#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../HEADERS/tuple.h"


uint64_t tuple_getkey ( tuple *this ) {
	return this->key ;
}

uint64_t tuple_getpayload ( tuple *this ) {
	return this->payload ;
}

void tuple_setkey ( tuple *this , uint64_t k ) {
	this->key = k;
}

void tuple_setpayload ( tuple *this , uint64_t p ) {
	this->payload = p;
};