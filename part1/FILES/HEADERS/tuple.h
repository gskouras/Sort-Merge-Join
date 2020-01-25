#include <stdint.h>


typedef struct tuple {
	unsigned long long key ;
	unsigned long long payload ;
} tuple;




unsigned long long tuple_getkey ( tuple * ) ;

unsigned long long tuple_getpayload ( tuple * ) ;

void tuple_setkey ( tuple * , unsigned long long ) ;

void tuple_setpayload ( tuple * , unsigned long long ) ;