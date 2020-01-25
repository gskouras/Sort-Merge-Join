#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../HEADERS/tuple.h"


unsigned long long tuple_getkey ( tuple *this ) {
	return this->key ;
}

unsigned long long tuple_getpayload ( tuple *this ) {
	return this->payload ;
}

void tuple_setkey ( tuple *this , unsigned long long k ) {
	this->key = k;
}

void tuple_setpayload ( tuple *this , unsigned long long p ) {
	this->payload = p;
};