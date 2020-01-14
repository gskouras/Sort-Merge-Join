
#include "../HEADERS/batch.h"
#include "../HEADERS/inbetween.h"



void print_check_sums( Check_sums *, all_data * , Between * );


void check ( void );


Batches * createBatches(char *filename, char *s)
{
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

void fillBatches(Batches *batches, char *s, char *filename)
{
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
	for (int i = 0; i < bl->size ; ++i)
	{
		execute_query(bl->batch[i], datatable);
	}
}

void execute_query(char * query, all_data *datatable)
{

    //printf("Query is %s\n",query );

	char rel_str[50]; 
	char pred_str[200];
	char check_sums_str[50];

	int *alias_array = NULL;
	int num_of_rel, num_of_pred, num_of_check_sums;
	Predicates predicates;
  Check_sums check_sums;

	char *token;
	token = strtok(query, "|");
	strcpy(rel_str, token);

	token = strtok(NULL, "|");
	strcpy(pred_str, token);

	token = strtok(NULL, "\n");
	strcpy(check_sums_str, token);

	num_of_rel = find_num_of_relations(rel_str); //find number of relations for current query;
	alias_array = (int *)malloc(sizeof(int) * num_of_rel); //give space to save the relations

  fill_relations(rel_str, alias_array); //fill array of relations

	num_of_pred = find_num_of_predicates(pred_str); //calculating number of predicates for current query
	predicates.predicates_array = malloc(sizeof(Predicate) * num_of_pred); 
	predicates.size = num_of_pred;

	fill_predicates(pred_str, &predicates, alias_array);//fill table of predicates

  num_of_check_sums = find_num_of_relations(check_sums_str);
  check_sums.check_sums_array = malloc(sizeof(Check_sum) * num_of_check_sums);
  check_sums.size = num_of_check_sums;


  fill_check_sums(check_sums_str, &check_sums, alias_array);//fill table of check_sums

  Between *b;
  b = exec_preds ( &predicates , datatable , b ) ;
  //result_list_print_nodes(b->r_list);
  print_check_sums( &check_sums, datatable , b);
  
  free(b);
  free(alias_array);
  free(predicates.predicates_array);
  free(check_sums.check_sums_array);

}


void print_check_sums( Check_sums *c , all_data * datatable , Between *b ) {

  //result_list_print_nodes ( b->r_list );



  int size = c->size;
  int rel_al ,rel_or , col_no;
  int result_size = b->r_list->total_results;
  //printf("total results are %d\n",result_size );
  int rowid;
  uint64_t sum = 0;
  int flag = 1;
;
  for ( int i = 0 ; i < size ; i++ ) {

    if ( b->r_list->root == NULL ){
      printf("NULL\t");
      flag = 0;
    }
    rel_al = c->check_sums_array[i].rel_alias;
    rel_or = c->check_sums_array[i].rel_origin;
    col_no = c->check_sums_array[i].rel_col;

    sum = between_get_sum ( b , datatable , rel_or , rel_al , col_no );
    

   if ( flag ) {
    printf("%ld\t", sum);
    sum = 0;
    }

  }
  printf("\n");
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
    //printf("Allias_array [%d] is %d\t", i, alias_array[i]);
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
    //printf("\npd.predicates_array[%d].rel1_alias is %d\n", i, pd->predicates_array[i].rel1_alias);

		pd->predicates_array[i].rel1_origin = alias_array[pd->predicates_array[i].rel1_alias];
    //printf("pd.predicates_array[%d].rel1_origin is %d\n", i,  pd->predicates_array[i].rel1_origin);

		token = strtok(NULL, "=><");
		pd->predicates_array[i].rel1_col = atoi(token);
    //printf("pd.predicates_array[%d].rel1_col is %d\n", i,  pd->predicates_array[i].rel1_col);

		token = strtok(NULL, ".&");

		if(isnumber(token))
		{
			pd->predicates_array[i].rel2_alias = -1;
			pd->predicates_array[i].filter_value = atoi(token);// na do  atoi gia unsinged int;
     // printf("pd.predicates_array[%d].filter_value is %ld\n", i,  pd->predicates_array[i].filter_value);
		}
		else
		{
			pd->predicates_array[i].rel2_alias = atoi(token);
      //printf("pd.predicates_array[%d].rel2_alias is %d\n", i,  pd->predicates_array[i].rel2_alias);

			pd->predicates_array[i].rel2_origin = alias_array[pd->predicates_array[i].rel2_alias];
      //printf("pd.predicates_array[%d].rel2_origin is %d\n", i,  pd->predicates_array[i].rel2_origin);

			token = strtok(NULL, "&");
			pd->predicates_array[i].rel2_col = atoi(token);
      //printf("pd.predicates_array[%d].rel2_col is %d\n", i,  pd->predicates_array[i].rel2_col);
		}
    //printf("pd.predicates_array[%d].op is %c\n\n", i,  pd->predicates_array[i].op);
		token = strtok(NULL, ".\n");
	}
}

void fill_check_sums(char* check_sums_str, Check_sums *cs, int *alias_array)
{
  char *token;
  size_t len = strlen(check_sums_str);
  char localstr[len+1];
  strcpy(localstr, check_sums_str);
  token = strtok(localstr, ".");

  for (int i = 0; i < cs->size; ++i)
  {
    cs->check_sums_array[i].rel_alias = atoi(token);
    //printf("check_sums[%d] rel_alias is %d\t", i, cs->check_sums_array[i].rel_alias);

    cs->check_sums_array[i].rel_origin = alias_array[cs->check_sums_array[i].rel_alias];
    //printf("check_sums[%d] rel_origin is %d\t", i, cs->check_sums_array[i].rel_origin);
    token = strtok(NULL, " ");

    cs->check_sums_array[i].rel_col = atoi(token);
    //printf("check_sums[%d] coloumn is %d\n", i, cs->check_sums_array[i].rel_col);

    token = strtok(NULL, ". ");
   }
}


int isnumber(char * number)
{
  if (strlen(number) > 1)
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


float instead_of_pow(float a, float b)
{
  return 0.1;
}

void restore_statistics (relation_data ** relations, int rel)
{
  int i,k;
  relation_data * reldata;
  if (rel!=-1)
    reldata = relations[rel];

  for (k=0;k<reldata->numColumns;k++)
  {
    reldata->columns[k]->l = reldata->columns[k]->prev_l;
    reldata->columns[k]->u = reldata->columns[k]->prev_u;
    reldata->columns[k]->f = reldata->columns[k]->prev_f;
    reldata->columns[k]->d = reldata->columns[k]->prev_d;
    reldata->columns[k]->restored=0;
  }

}


float update_statistics(relation_data ** relations, Predicate ** predicate, int pred_num)
{
  //first keep old values in case we need to restore them
  //for table A
  Predicate * curr_pred;
  curr_pred = (Predicate*)malloc(sizeof(Predicate));
  curr_pred->rel1_alias = predicate[pred_num]->rel1_alias;
  curr_pred->rel1_col = predicate[pred_num]->rel1_col;
  curr_pred->rel2_alias= predicate[pred_num]->rel2_alias;
  curr_pred->rel2_col = predicate[pred_num]->rel2_col;
  curr_pred->op = predicate[pred_num]->op;
  int curr_column = curr_pred->rel1_col;
  int k;
  float old_d,second_old_d;
  relation_data * reldata,* second_reldata;

  if (curr_pred->rel1_alias!=-1)
    {reldata = relations[curr_pred->rel1_alias];

  if (curr_pred->rel2_alias!=-1)
  { 
    second_reldata = relations[curr_pred->rel2_alias];
  }
  else
  {
    second_reldata = NULL;
  }
  int i = curr_column;
  float n = (reldata->columns[i]->u - reldata->columns[i]->l +1);
  float old_f= reldata->columns[i]->f;//used for storing previous f value

  for (k=0;k<reldata->numColumns;k++)
  {
    if (reldata->columns[k]->restored==0)//store data only if its the first call. this protects that from storing intermediate results in the restoring section
    {
    reldata->columns[k]->prev_l =reldata->columns[k]->l;
    reldata->columns[k]->prev_u =reldata->columns[k]->u;
    reldata->columns[k]->prev_f =reldata->columns[k]->f;
    reldata->columns[k]->prev_d =reldata->columns[k]->d;
    reldata->columns[k]->restored=1;
    }
  }
  if (curr_pred->rel2_alias==-1 && curr_pred->op=='=')//R.A = k
  {
    if (reldata->columns[i]->u-reldata->columns[i]->l+1<M) //we have a tabe of size u- l+1
    {
      if (curr_pred->rel2_col < reldata->columns[i]->l || curr_pred->rel2_col > reldata->columns[i]->u )
      {
        reldata->columns[i]->d=0.0;
        reldata->columns[i]->f=0.0;
      }
      else if (reldata->columns[i]->d_table[(int)(curr_pred->rel2_col - reldata->columns[i]->l)]==1)
      {
  //      printf("%d exists in d_table\n",curr_pred->rel2_col);
        reldata->columns[i]->f=reldata->columns[i]->f/reldata->columns[i]->d;
        reldata->columns[i]->d=1.0;
      }
      else
        {
          reldata->columns[i]->d=0.0;
          reldata->columns[i]->f=0.0;
        }
    }
    else //we have a table of size M
    {
      if (reldata->columns[i]->d_table[(int)(curr_pred->rel2_col-reldata->columns[i]->u)%M]==1)
        {
  //        printf("%d exists in d_table\n",curr_pred->rel2_col);
          reldata->columns[i]->f=reldata->columns[i]->f/reldata->columns[i]->d;
          reldata->columns[i]->d=1.0;
        }
        else
          {
            reldata->columns[i]->d=0.0;
            reldata->columns[i]->f=0.0;
          }
    }
    reldata->columns[i]->l = curr_pred->rel2_col;
    reldata->columns[i]->u = curr_pred->rel2_col;
    //now for the rest columns
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;
      reldata->columns[i]->d = reldata->columns[i]->d * (1.0 -instead_of_pow((1.0- (reldata->columns[curr_column]->f/old_f)),reldata->columns[i]->f/reldata->columns[i]->d));
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
  }
  else if (curr_pred->rel2_alias == -1 && curr_pred->op=='>')//R.A > k
  {
    float k1 = curr_pred->rel2_col;
    if (k1<reldata->columns[i]->l)
        k1 = reldata->columns[i]->l;

    reldata->columns[i]->l =k1;
    reldata->columns[i]->d = reldata->columns[i]->d * (reldata->columns[i]->u-k1)/(reldata->columns[i]->u-reldata->columns[i]->l);
    reldata->columns[i]->f = reldata->columns[i]->f * (reldata->columns[i]->u-k1)/(reldata->columns[i]->u-reldata->columns[i]->l);
    //now for the rest colmns
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;

      reldata->columns[i]->d = reldata->columns[i]->d * (1.0 -instead_of_pow((1.0- (reldata->columns[curr_column]->f/old_f)),reldata->columns[i]->f/reldata->columns[i]->d));
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
  }
  else if (curr_pred->rel2_alias== -1 && curr_pred->op=='<')//R.A < k
  {
    float k2 = curr_pred->rel2_col;
    if (k2>reldata->columns[i]->u)
      k2 = reldata->columns[i]->u;

    reldata->columns[i]->u = k2;
    reldata->columns[i]->d = reldata->columns[i]->d * (k2-reldata->columns[i]->l)/(reldata->columns[i]->u-reldata->columns[i]->l);
    reldata->columns[i]->f = reldata->columns[i]->f * (k2-reldata->columns[i]->l)/(reldata->columns[i]->u-reldata->columns[i]->l);
    //now for the rest colums
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;

      reldata->columns[i]->d = reldata->columns[i]->d * (1.0 -instead_of_pow((1.0- (reldata->columns[curr_column]->f/old_f)),reldata->columns[i]->f/reldata->columns[i]->d));
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
  }
  else if (curr_pred->rel1_alias == curr_pred->rel2_alias)//R.A = R.B
  {
    if (reldata->columns[i]->l<reldata->columns[curr_pred->rel2_col]->l)
    {
      reldata->columns[i]->l = reldata->columns[curr_pred->rel2_col]->l;
    }
    else
    {
      reldata->columns[curr_pred->rel2_col]->l = reldata->columns[i]->l;
    }
    if (reldata->columns[i]->u<reldata->columns[curr_pred->rel2_col]->u)
    {
      reldata->columns[curr_pred->rel2_col]->u = reldata->columns[i]->u;
    }
    else
    {
      reldata->columns[i]->u = reldata->columns[curr_pred->rel2_col]->u;
    }
    reldata->columns[i]->f = reldata->columns[i]->f/n;
    reldata->columns[curr_pred->rel2_col]->f =  reldata->columns[i]->f;
    reldata->columns[i]->d = reldata->columns[i]->d * (1.0 -instead_of_pow((1.0- (reldata->columns[curr_column]->f/old_f)),(reldata->columns[i]->f/reldata->columns[i]->d)));
    reldata->columns[curr_pred->rel2_col]->d = reldata->columns[i]->d;
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;

      reldata->columns[i]->d = reldata->columns[i]->d * (1.0 -instead_of_pow((1.0- (reldata->columns[curr_column]->f/old_f)),reldata->columns[i]->f/reldata->columns[i]->d));
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
  }
  else if ((curr_pred->rel1_alias == curr_pred->rel2_alias) && (curr_pred->rel1_col == curr_pred->rel2_col)) //autosisxetisi
  {
    reldata->columns[i]->f = reldata->columns[i]->f * reldata->columns[i]->f /n;
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;;
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
  }
  else //join metaksi 2 pinakon
  {

    //for table B
    for (k=0;k<second_reldata->numColumns;k++)
    {
      if (second_reldata->columns[k]->restored==0)//store data only if its the first call. this protects that from storing intermediate results in the restoring section
      {
      second_reldata->columns[k]->prev_l =second_reldata->columns[k]->l;
      second_reldata->columns[k]->prev_u =second_reldata->columns[k]->u;
      second_reldata->columns[k]->prev_f =second_reldata->columns[k]->f;
      second_reldata->columns[k]->prev_d =second_reldata->columns[k]->d;
      second_reldata->columns[k]->restored=1;
      }
    }
    //dialego apo ta 2 low to megalitero low, kai apo ta 2 upper to mikrotero upper
    if (reldata->columns[i]->l<second_reldata->columns[curr_pred->rel2_col]->l)
    {
      reldata->columns[i]->l = second_reldata->columns[curr_pred->rel2_col]->l;
    }
    else
    {
      second_reldata->columns[curr_pred->rel2_col]->l = reldata->columns[i]->l;
    }
    if (reldata->columns[i]->u<second_reldata->columns[curr_pred->rel2_col]->u)
    {
      second_reldata->columns[curr_pred->rel2_col]->u = reldata->columns[i]->u;
    }
    else
    {
      reldata->columns[i]->u = second_reldata->columns[curr_pred->rel2_col]->u;
    }
    
    float n = (float)(reldata->columns[i]->u - reldata->columns[i]->l +1);
    reldata->columns[i]->f= reldata->columns[i]->f * second_reldata->columns[curr_pred->rel2_col]->f/n;
    second_reldata->columns[curr_pred->rel2_col]->f = reldata->columns[i]->f;
    old_d = reldata->columns[i]->d;
    reldata->columns[i]->d = reldata->columns[i]->d  * second_reldata->columns[curr_pred->rel2_col]->d/n;
    second_old_d = second_reldata->columns[curr_pred->rel2_col]->d;
    second_reldata->columns[curr_pred->rel2_col]->d = reldata->columns[i]->d;
    //now for the rest columns
    //for table A
    for (i=0;i<reldata->numColumns;i++)
    {
      if (i==curr_column)
        continue;
      //printf("\nd of i column is %f| d of curr_column %f| old_d is %f| f of i columns is %f | d of i column is %f\n", reldata->columns[i]->d , reldata->columns[curr_column]->d,old_d,reldata->columns[i]->f,reldata->columns[i]->d);
      reldata->columns[i]->d = reldata->columns[i]->d * (1.0 - instead_of_pow((1.0 - (reldata->columns[curr_column]->d/old_d)),reldata->columns[i]->f/reldata->columns[i]->d));
      //printf("Thus, new d is %f\n",reldata->columns[i]->d );
      reldata->columns[i]->f = reldata->columns[curr_column]->f;
    }
    //for table B
    for (i=0;i<second_reldata->numColumns;i++)
    {
    //printf("Num of colums is %ju and i is %d\n",second_reldata->numColumns,i);
      if (i==curr_pred->rel2_col)
        continue;
      //printf("(second) d of i column is %f| d of curr_column %f| old_d is %f| f of i columns is %f | d of i column is %f, instead_of_pow is %f\n", second_reldata->columns[i]->d,second_reldata->columns[curr_column]->d,second_old_d,second_reldata->columns[i]->f,second_reldata->columns[i]->d,instead_of_pow((1.0 - (second_reldata->columns[curr_column]->d/second_old_d)),second_reldata->columns[i]->f/second_reldata->columns[i]->d));
      second_reldata->columns[i]->d = second_reldata->columns[i]->d * (1.0 -instead_of_pow((1.0 - (second_reldata->columns[curr_column]->d/second_old_d)),second_reldata->columns[i]->f/second_reldata->columns[i]->d));
      second_reldata->columns[i]->f = second_reldata->columns[curr_column]->f;
    //printf("(second) Thus, new d is %f\n",second_reldata->columns[i]->d );
    }
  }

  }
  //printf("Statistics Updated! New stats:\n Low %ju \n Upper %ju \n Arithmos Stoixeion %f \n Arithmos Monadikon Stoixeion %f\n",reldata->columns[curr_column]->l, reldata->columns[curr_column]->u,reldata->columns[curr_column]->f,reldata->columns[curr_column]->d);
  return reldata->columns[curr_column]->f;
  //sto join metaksi 2 pinakwn, h prosvasi ston deutero pinaka na ginetai meso ths second_reldata. na kanw tis katalliles allages
}


void free_all_batches(Batches * batches)
{
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

