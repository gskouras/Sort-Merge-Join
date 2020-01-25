#include "../HEADERS/list.h"


Listnode* initListNode(List *list)
{
	Listnode *new_node = malloc(sizeof(Listnode));
	new_node->tuples = malloc(sizeof(tuple) * list->tuples_size);
	new_node->current_position = 0;
	new_node->next = NULL;
	return new_node;
}

void initlist(List *list, int maxbytes, int record_size)
{
	list->head= NULL;
	list->tail = NULL;
	list->counter = 0;
	list->tuples_size = (maxbytes/record_size);
}

int isEmpty(List *list)
{
	return list->counter == 0;
}

void insertNewNode(List *list)
{
	Listnode *new_node = initListNode(list);
	if(isEmpty(list))
	{
		list->counter = 1;
		list->head = new_node;
		list->tail = new_node;
		return;
	}
	list->counter++;
	list->tail->next = new_node;
	list->tail = new_node;
}

void insertElement(List *list, tuple tuple1, tuple tuple2)
{
	if(isEmpty(list))
	{
		insertNewNode(list);
	}
	if(list->tuples_size == list->tail->current_position)//an xoraei kainourgio record ston komvo tis listas
	{
		insertNewNode(list);
	}
	list->tail->tuples[list->tail->current_position].key = tuple1.payload;
	list->tail->tuples[list->tail->current_position].payload = tuple2.payload;
	list->tail->current_position++;

}

void join(relation *relR, relation *relS, List *list){
	
	int i, k = 0;
	int prevk = 0;

	for(i = 0; i<relR->num_tuples; i++) 
	{
		if (i > 0 && isEqual(relR, relS, i, i-1))
		{
			k = prevk;
		}else{
			prevk = k;
	    }

		if(k == relS->num_tuples) break;

		if(isEqual(relR, relS, i, k))
		{
			while (isEqual(relR, relS, i, k))
			{
				insertElement(list, relR->tuples[i], relS->tuples[k]);
				k++;
			}
		}else
		{
			for( int64_t j = k; j < relS->num_tuples; j++)
			{
				if(isEqual(relR, relS, i, j)) insertElement(list, relR->tuples[i], relS->tuples[j]);
			}
		}	
	}
    printList(list);
}

void printList(List *list)
{
	Listnode *tmp = list->head;
	FILE *fp;
	fp = fopen("Join_Results.txt", "w");
	printf(":::Sort_Merge_Join Completed Succesfully:::");

	if(isEmpty(list))
	{
		printf("The list doesnt't contain any equal relation, it is Empty\n");
	}
	// //REMOVE COMMENTS IF YOU WANT TO PRINT IN FILE

	while(tmp != NULL)
	{
		// printTuples(tmp);
		
		for(int i=0;i<tmp->current_position;i++)
		{
			fprintf(fp, "(%llu, %llu)\n", tmp->tuples[i].key, tmp->tuples[i].payload);
		}
		tmp = tmp->next;
	}
	
	printf("\n\n");
	printf("The list created to hold the results contains %d equal relations divided into %d nodes.\n", ((list->tuples_size * --list->counter) + list->tail->current_position), list->counter);
	printf("Each node contains %d relations\n\n", list->tuples_size);
	freeList(list);
}

void printTuples(Listnode *node)
{
	int i;
	FILE *fp;
	fp = fopen("Join_Results.txt", "w");
	for(i=0;i<node->current_position;i++)
	{
		fprintf(fp, "(%llu, %llu)\n", node->tuples[i].key, node->tuples[i].payload);
	}
	fclose(fp);
}

void freeListNode(Listnode *listnode)
{
	free(listnode->tuples);
}

void freeList(List *list)
{
	Listnode *tmp;
	while(list->head != NULL)
	{
		tmp = list->head;
		list->head = list->head->next;
		freeListNode(tmp);
		free(tmp);
	}
}

void printRelation(relation *rel)
{
    for (int i=0; i<10; i++)
    {
    	printf("%llu\t", rel->tuples[i].key);
        printf("%llu ", rel->tuples[i].payload);
        printf("\n");
    }
    printf("\n\n");
}

int isEqual(relation *relR, relation *relS, int i, int k)
{
	return relR->tuples[i].key == relS->tuples[k].key;
}

int getListTotalRel(List *list)
{
	return (list->tuples_size * --list->counter) + list->tail->current_position;
}




