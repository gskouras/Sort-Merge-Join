#ifndef _TUPLE_H_
#define _TUPLE_H_


#include <stdint.h>


typedef struct tuple {
	uint64_t key ;
	uint64_t payload ;
} tuple;



uint64_t tuple_getkey ( tuple * ) ;

uint64_t tuple_getpayload ( tuple * ) ;

void tuple_setkey ( tuple * , uint64_t ) ;

void tuple_setpayload ( tuple * , uint64_t ) ;


#endif