#include "../HEADERS/batch.h"

Batches * createBatches(char *filename, char *s){
  int count1 = 0, count2 = 0, c;
  memset(s,0,sizeof(s));
  FILE *fp = fopen(filename, "r");
  for (c = getc(fp); c != EOF; c = getc(fp)){
    if(c == '\n'){
      count2++;
    }
        if (c == 'F'){
          count1++; 
          sprintf(s,"%s%d_",s, count2);
          count2 = -1;
        } 
    }
    // Close the file 
    fclose(fp); 
    Batches *batches = malloc(sizeof(Batches));
    batches->size = count1;
    batches->batches = malloc(sizeof(Batch_lines)*batches->size);

    return batches;
}

void fillBatches(Batches *batches, char *s, char *filename){
  int count, n, i, k;
  char *line = NULL;
  size_t len = 0;
    ssize_t read;
  char * token = strtok(s, "_");
  count = atoi(token);
  FILE *fp = fopen(filename, "r");

  for (i = 0; i < batches->size; ++i)
  {
    batches->batches[i].batch = malloc(sizeof(char*)*count);
    batches->batches[i].size = count;
    for (k = 0; k < count; ++k)
    {
      getline(&line, &len, fp);
      batches->batches[i].batch[k] = malloc(sizeof(char)*strlen(line)+1);
      strcpy(batches->batches[i].batch[k], line);
      //printf("%s",batches->batches[i].batch[k]);
    }
    getline(&line, &len, fp); // einai to F
    //printf("%s", line);
    if(i == batches->size-1)
      continue;
    token = strtok(NULL, "_");
    count = atoi(token);
  }


  fclose(fp);

  if (line)
        free(line);
}


void execute_all_batches(char *filename, all_data *datatable)
{

	char str[100];
	Batches *batches = createBatches("small.work", str);
	//printf("%d\n", batches->size);
	//printf("%s\n",str);
	fillBatches(batches, str, "small.work");

  for (int i = 0; i < batches->size; ++i)
  {
    execute_batch(&batches->batches[i], datatable);
  }
  
	free_all_batches(batches);
  	return;
}




void execute_batch(Batch_lines * bl, all_data * datatable )
{
	for (int i = 0; i < bl->size; ++i)
	{
		execute_query(bl->batch[i], datatable);
	}
}

void execute_query(char * query, all_data *datatable)
{
  printf(" char query is %s\n",query );
	char rel_str[50]; 
	char pred_str[200];
	char check_sums_str[50];

	int *alias_array = NULL;
	int num_of_rel, num_of_pred; 
	Predicates predicates;

	char *token;
	token = strtok(query, "|");
	strcpy(rel_str, token);

	token = strtok(NULL, "|");
	strcpy(pred_str, token);

	token = strtok(NULL, "\n");
	strcpy(check_sums_str, token);

	num_of_rel = find_num_of_relations(rel_str);
	alias_array = (int *)malloc(sizeof(int) * num_of_rel);

	num_of_pred = find_num_of_predicates(pred_str);
	predicates.predicates_array = malloc(sizeof(Predicate) * num_of_pred);
	predicates.size = num_of_pred;

  fill_relations(rel_str, alias_array);
	fill_predicates(pred_str, &predicates, alias_array);


}

void fill_relations(char *rel_str, int *alias_array)
{
  int size = find_num_of_relations(rel_str);
  char *token;
  size_t len = strlen(rel_str);
  char localstr[len+1];
  strcpy(localstr, rel_str);
  token = strtok(localstr, " ");
  int i = 0;

  while(token)
  {
    alias_array[i] = atoi(token);
    token = strtok(NULL, " ");
    printf("Allias_array [%d] is %d\t", i, alias_array[i]);
    i++;
  }

}


void fill_predicates(char *pred_str, Predicates *pd, int *alias_array)
{
	int index = 0;
	for (int i = 0; i < strlen(pred_str); ++i)
	{
		if(pred_str[i] == '=' || pred_str[i] == '<' || pred_str[i] == '>')
		{
			pd->predicates_array[index++].op = pred_str[i];
		}
	}

	char *token = strtok(pred_str, ".");

	for (int i = 0; i < pd->size; ++i)
	{
		pd->predicates_array[i].flag_exec = 0;

		pd->predicates_array[i].rel1_alias = atoi(token);
    printf("\npd.predicates_array[%d].rel1_alias is %d\n", i, pd->predicates_array[i].rel1_alias);

		pd->predicates_array[i].rel1_origin = alias_array[pd->predicates_array[i].rel1_alias];
    printf("pd.predicates_array[%d].rel1_origin is %d\n", i,  pd->predicates_array[i].rel1_origin);

		token = strtok(NULL, "=><");
		pd->predicates_array[i].rel1_col = atoi(token);
    printf("pd.predicates_array[%d].rel1_col is %d\n", i,  pd->predicates_array[i].rel1_col);

		token = strtok(NULL, ".&");

		if(isnumber(token))
		{
			pd->predicates_array[i].rel2_alias = -1;
			pd->predicates_array[i].filter_value = atoi(token);// na do  atoi gia unsinged int;
      printf("pd.predicates_array[%d].filter_value is %ld\n", i,  pd->predicates_array[i].filter_value);
		}
		else
		{
			pd->predicates_array[i].rel2_alias = atoi(token);
      printf("pd.predicates_array[%d].rel2_alias is %d\n", i,  pd->predicates_array[i].rel2_alias);

			pd->predicates_array[i].rel2_origin = alias_array[pd->predicates_array[i].rel2_alias];
      printf("pd.predicates_array[%d].rel2_origin is %d\n", i,  pd->predicates_array[i].rel2_origin);

			token = strtok(NULL, "&");
			pd->predicates_array[i].rel2_col = atoi(token);
      printf("pd.predicates_array[%d].rel2_col is %d\n", i,  pd->predicates_array[i].rel2_col);
		}
    printf("pd.predicates_array[%d].op is %c\n\n", i,  pd->predicates_array[i].op);
		token = strtok(NULL, ".\n");
	}
}

int isnumber(char * number)
{
  if (strlen(number) > 2)
  {
    return 1;
  }	
	return 0;
}

int find_num_of_relations(char * rel_line)
{
	int count= 0;
	for (int i = 0; i < strlen(rel_line); ++i)
	{
		if ( rel_line[i] == ' ')
			count++;
	}
	return ++count;
}

int find_num_of_predicates(char * pred_line)
{
	int count= 0;
	for (int i = 0; i < strlen(pred_line); ++i)
	{
		if ( pred_line[i] == '&')
			count++;
	}
	return ++count;
}


void free_all_batches(Batches * batches){
	int i, k;
	for (int i = 0; i < batches->size; ++i)
	{
		for (k = 0; k < batches->batches[i].size; ++k){
			free(batches->batches[i].batch[k]);
		}
		free(batches->batches[i].batch);
	}
	free(batches->batches);
	free(batches);
}

