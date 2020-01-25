#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../HEADERS/relation.h"

#define MAXSIZE 1048576

typedef struct ln
{
	struct tuple *tuples;
	int current_position;
	struct ln *next;
}Listnode;

typedef struct list
{
	Listnode *head;
	Listnode *tail;
	int counter;
	int tuples_size;
}List;


void join(relation *relR, relation *relS,  struct list *list);

void initlist(List *list, int maxbytes, int record_size);

void insertElement(List *list, struct tuple tuple1, struct tuple tuple2);

int isEmpty(List *list);

void insertNewNode(List *list);

Listnode* initListNode(List *list);

void printList(List *list);

void freeListNode(Listnode *listnode);

void freeList(List *list);

void join(relation *relR, relation *relS,  struct list *list);

void printRelation(relation * ) ;

int isEqual(relation *, relation *, int , int );

int getListTotalRel(List *list);

#endif
