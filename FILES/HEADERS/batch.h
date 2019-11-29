#ifndef _BATCH_H
#define _BATCH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "../HEADERS/predicates.h"


typedef struct query{
  char * sxeseis;
  char * proboles;
  predicates ** katigorimata;
  int num_of_predicates;
  int num_of_relations;
} query;


typedef struct batch{
    query ** queries_table;
    int num_of_queries;
} batch;

void seperate_query(char *, char * temp_query_pieces[3]);

batch *InitBatch(); //Initialization of new Batch

batch *AddToBatch(); //Insert new query in current batch

void FreeBatch(batch *); //Free the allocated memory of current batch



#endif