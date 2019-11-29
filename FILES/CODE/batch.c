#include "../HEADERS/batch.h"




void seperate_query(char *filename, char * temp_query_pieces[3])
{
  char *query_pieces[3];//we need three pieces because the query is seperated into three parts.

  //take the relations involved in the query
  query_pieces[0] = strtok(filename,"|");
  temp_query_pieces[0] = malloc(strlen(query_pieces[0]) +1);
  strcpy(temp_query_pieces[0], query_pieces[0]);

  //then take the predicates involved
  query_pieces[1] = strtok(NULL ,"|");
  temp_query_pieces[1] = malloc(strlen(query_pieces[1]) +1);
  strcpy(temp_query_pieces[1], query_pieces[1]);

  //and last take the projections
  query_pieces[2] = strtok(NULL ,"|");
  temp_query_pieces[2] = malloc(strlen(query_pieces[2]) +1);
  strcpy(temp_query_pieces[2], query_pieces[2]);

}



batch *InitBatch()
{
  batch *this_batch=malloc(sizeof(batch));
  this_batch->num_of_queries=0;
  this_batch->queries_table=malloc(sizeof(query*));
}

batch *AddToBatch(batch *this_batch,char **tokens){
  int num_predicates;
  int num_relations=1;
  for(int j=0;j<=strlen(tokens[0]);j++){ //relations are seperated my space, so the amount of spaces are the amount of relations
    if (isspace(tokens[0][j])){
      num_relations++;
    }
  }
  this_batch->queries_table = realloc(this_batch->queries_table,sizeof(query*)*(this_batch->num_of_queries+1));
  if (this_batch->queries_table==NULL)
  {
  	    printf("Error on reallocing memory for this query!\n");
  }

  this_batch->queries_table[this_batch->num_of_queries] = (query *)malloc(sizeof(query));
  this_batch->queries_table[this_batch->num_of_queries]->sxeseis = tokens[0];
  this_batch->queries_table[this_batch->num_of_queries]->proboles = tokens[2]; 

  num_predicates = calc_predicates_num(tokens[1]);

  this_batch->queries_table[this_batch->num_of_queries]->num_of_predicates = num_predicates;
  this_batch->queries_table[this_batch->num_of_queries]->katigorimata  = fill_predicates(tokens[1],num_predicates);
  this_batch->queries_table[this_batch->num_of_queries]->num_of_relations = num_relations;
  this_batch->num_of_queries++;
}


void FreeBatch(batch *this_batch){

  if(this_batch==NULL) return ;

  for(int k=0;k<this_batch->num_of_queries;k++)
  {
    free(this_batch->queries_table[k]->sxeseis);
    free(this_batch->queries_table[k]->proboles);
    for(int i=0;i<this_batch->queries_table[k]->num_of_predicates;i++)
    {
      free(this_batch->queries_table[k]->katigorimata[i]);
    }
    free(this_batch->queries_table[k]->katigorimata);
  }
  free(this_batch);
}