#ifndef _PREDICATES_H__
#define _PREDICATES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct predicates{      /* struct predicates stores info for predicate in query*/
  int rel1;             /* index of 1st relation in predicate*/
  int col1;             /* index of column of 1st relation in predicate*/
  int rel2;             /* index of 2nd relation in predicate*/
  int col2;             /* index of column of 2nd relation in predicate*/
  char op;           /* operation between the 2 columns */
} predicates;


int calc_predicates_num(char *); //calculating the number of predicates in a given query;


predicates **fill_predicates(char * , int);


void print_predicates (predicates ** predicates, int num_predicates); //print the predicates of a query







#endif