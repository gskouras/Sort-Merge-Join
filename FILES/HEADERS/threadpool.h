#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>
#include "../HEADERS/job_list.h"


//STRUCT

typedef struct {
	job_list *queue;
} threadpool;



void  execute_all_batches_threads ( char *filename , all_data *datatable , threadpool *thp );


//CONSTRUCTOR

threadpool *threadpool_create ( threadpool * );


#endif