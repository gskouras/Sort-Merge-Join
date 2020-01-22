#include "../HEADERS/query.h"

char *execute_query ( Predicates *predicates , Check_sums *check_sums , all_data *datatable ) {

    Between *b;
    b = exec_preds ( predicates , datatable , b );

    char *line = check_sums_generate_string ( check_sums, datatable , b );
    //printf("%s\n", line);
    int total_rels = calculate_relations(predicates);
    for ( int i = 0 ; i < total_rels ; i++ ) {
      if ( b->already_joined[i] == 1) {
        free(b->jarrays[i]);
      }
    }

    //SERIAL/////////////
    printf("%s\n", line);
    /////////////////////
    free(b->jarrays);
    free(b->farrays);
    free(b);
    return line;
}



char *check_sums_generate_string( Check_sums *c , all_data * datatable , Between *b ) {

  //result_list_print_nodes ( b->r_list );
  char *line = malloc(200);
  char *val = malloc(200);
  int total_len = 0;

  int size = c->size;
  int rel_al ,rel_or , col_no;
  int rowid;
  uint64_t sum = 0;
  int flag = 1;

  for ( int i = 0 ; i < size ; i++ ) {

    if ( b->jarrays_size == 0 ){


      memcpy(line + total_len , "NULL\t" , strlen("NULL\t") + 1);
      total_len += strlen("NULL\t");

    }
    else {
    rel_al = c->check_sums_array[i].rel_alias;
    rel_or = c->check_sums_array[i].rel_origin;
    col_no = c->check_sums_array[i].rel_col;

    for ( int s = 0 ; s < b->jarrays_size ; s++ ) {
      rowid = b->jarrays[rel_al][s];
      sum += datatable->table[rel_or]->columns[col_no]->tuples[rowid].payload;
    }
    

    if ( flag ) {

    sprintf(val, "%ld\t", sum);
    memcpy(line + total_len , val , strlen(val) + 1 );
    total_len += strlen(val);

    sum = 0;
    }
  }
  }
  return line;
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


uint64_t calculate_relations ( Predicates *pd ) 
{
  int used[pd->size];
  uint64_t count = 0 ;
  int rel1,rel2; //Here we store the rel origin
  int flag ;

  for (int i = 0; i < pd->size ; i++ ) 
  {
    rel1 = pd->predicates_array[i].rel1_alias;
    flag = in_used_relation ( used , count , rel1 );  //When flag is 1 that means rel is not already use so we add it to used array
    if ( flag ) 
    {
      used[count] = rel1;
      count++;
    }
    rel2 = pd->predicates_array[i].rel2_alias;
    if ( rel2 != -1 ) 
    {
      flag = in_used_relation ( used , count , rel2);
      if ( flag ) 
      {
        used[count] = rel2;
        count++;
      }
    }
  }

  return count;
}



//BETWEEN


//JOINS

Between *exec_preds ( Predicates *pd, all_data *datatable , Between *b ) {

  printf("\n");
  int psize = pd->size;
  int *cur_array;
  int fflag;
  //First calculate how many relations we are going to use
  uint64_t total_rels = calculate_relations ( pd );
  b = create_between ( b , total_rels);


  int filtered[total_rels]; //Keep which rels we filtered
  int filter_count = 0;
  Predicate *temp_pred;
  //First execute filter predicates
  for (int i = 0 ; i < psize; i++)
  {
    temp_pred = &pd->predicates_array[i];
    if ( temp_pred->rel2_alias == -1 )
    { //This means that we have a filter predicate
      //execute filter
      cur_array = execute_filter( pd , datatable , temp_pred , cur_array );
      b->farrays[temp_pred->rel1_alias] = cur_array;
      //Add to filtered
      fflag = in_used_relation( filtered , filter_count , temp_pred->rel1_alias);
      if ( fflag ) 
      {
        filtered[filter_count++] = temp_pred->rel1_alias;
      }
    }
  }
  int *flags;
  //Now execute the joins
  for ( int i = 0 ; i < psize ; i++ ) {
    temp_pred = &pd->predicates_array[i];
    if ( temp_pred->rel2_alias != -1 ) {
      int rel1_alias = temp_pred->rel1_alias;
      int rel2_alias = temp_pred->rel2_alias;
      flags = generate_flags ( flags , filtered , filter_count , rel1_alias , rel2_alias );
      b = execute_join ( b , flags , temp_pred , datatable , total_rels );
      b->already_joined[rel1_alias] = 1;
      b->already_joined[rel2_alias] = 1;
    }
  }
  return b;
}

Between *execute_join ( Between *b , int *flags , Predicate *temp_pred , all_data *datatable , int total_rels ) {
  int rel1_alias,rel2_alias,rel1_origin,rel2_origin,col1_no,col2_no;
  rel1_alias = temp_pred->rel1_alias;
  rel2_alias = temp_pred->rel2_alias;
  rel1_origin = temp_pred->rel1_origin;
  rel2_origin = temp_pred->rel2_origin;
  col1_no = temp_pred->rel1_col;
  col2_no = temp_pred->rel2_col;
  relation *result;

  //This function returns a relation built from filtered (or joined) if it was filterered(or joined) or from the datatable if not
  relation *r1 = prepare_relation ( b , datatable , rel1_origin , rel1_alias , col1_no , flags[0] ); 
  relation *r2 = prepare_relation ( b , datatable , rel2_origin , rel2_alias , col2_no , flags[1] );

  if (( b->already_joined[rel1_alias] == 1 && b->already_joined[rel2_alias] == 1) || (rel1_alias == rel2_alias)){
    b = scan ( b , datatable , rel1_alias , rel2_alias ,col1_no , col2_no , rel1_origin , rel2_origin , total_rels );
  }
  else if ( rel1_origin == rel2_origin ) {
    result = join ( r1 , r2 );
    
    b = between_update ( b , result , rel1_alias , rel2_alias , total_rels );

  }
  else{
    if ( rel1_origin != rel2_origin )
     result = join ( r1 , r2 );
     b = between_update ( b , result , rel1_alias , rel2_alias , total_rels );
  }

  //free(r1);
  //free(r2);
  //free(result);
  return b;
}

Between *scan ( Between *b , all_data *datatable , int rel1 , int rel2 , int col1 , int col2 , int r1_or , int r2_or , int total_rels ) {
  int rowid1,rowid2;
  int count = 0;
  int new_size = calculate_scan_size ( b->jarrays[rel1] , datatable , b->jarrays[rel2] , col1 , col2 , b->jarrays_size , r1_or , r2_or ); 
  uint64_t **new_jarrays = malloc ( total_rels * sizeof( uint64_t *) );
  uint64_t **jarrays_to_delete;
  for ( int i = 0 ; i < total_rels ; i++ ) {
    if ( b->already_joined[i] == 1 ) {
      new_jarrays[i] = malloc ( new_size * sizeof(uint64_t) ) ;
    }
  }
  for ( int i = 0 ; i < b->jarrays_size ; i++) {
    rowid1 = b->jarrays[rel1][i];
    rowid2 = b->jarrays[rel2][i];
    if ( datatable->table[r1_or]->columns[col1]->tuples[rowid1].payload == datatable->table[r2_or]->columns[col2]->tuples[rowid2].payload ){
      new_jarrays = fill_already_joined_rels ( b , new_jarrays , i , count , total_rels );
      count++;
    }
  }

  //INSERT NEW JARRAYS TO BETWEEN STRUCT
  jarrays_to_delete = b->jarrays;
  b->jarrays = new_jarrays;
  b->jarrays_size = new_size;
  //DELETE OLD JARRAYS 
  for ( int i = 0 ; i < total_rels ; i++ ){
    if ( b->already_joined[i] == 1 ) {
      free(jarrays_to_delete[i]);
    }
  }
  free(jarrays_to_delete);

  return b;
}

int calculate_scan_size ( uint64_t *array1 , all_data *datatable , uint64_t *array2 , int col1 , int col2 , int jsize , int rel1 , int rel2 ) {
  int rowid1,rowid2;
  int size = 0;
  for ( int i = 0 ; i < jsize ; i++) {
    rowid1 = array1[i];
    rowid2 = array2[i];
    if ( datatable->table[rel1]->columns[col1]->tuples[rowid1].payload == datatable->table[rel2]->columns[col2]->tuples[rowid2].payload ){
      size++;
    }
  }
  
  return size;
}


  Between *between_update ( Between *b , relation *result , int rel1_alias , int rel2_alias , int total_rels ) {

    int rel_ref,rel_add;
    int new_size;
    int flag ;


    //FIRST CHEK IF WE HAVE ANYTIHNG ALREADY JOINED 
    if ( b->jarrays_size == 0 ) {
      b = between_fill_empty( b , result , rel1_alias , rel2_alias );
    }
    else{
      if ( b->already_joined[rel1_alias] == 1 && b->already_joined[rel2_alias] != 1 ) {
        rel_ref = rel1_alias;
        rel_add = rel2_alias;
        flag = 0;
      }
      else if ( b->already_joined[rel2_alias] == 1 && b->already_joined[rel1_alias] != 1 ) {
        rel_ref = rel2_alias;
        rel_add = rel1_alias;
        flag = 1;
      }
      new_size = calculate_new_between_size ( b->jarrays[rel_ref] , result , b->jarrays_size , flag );
      b = fill_updated_jarrays ( b , new_size , result ,rel_ref , rel_add , total_rels , flag );


    }
    return b;
  }
  
  Between *fill_updated_jarrays ( Between *b , int new_size , relation *result , int rel_ref , int rel_add , int total_rels , int flag ){

    int ssize = 0;
    int similar = 0;
    //FIRST CREATE NEW JARRAYS STRUCT WITH THE GIVEN NEW_SIZE
    uint64_t **new_jarrays = malloc ( total_rels * sizeof( uint64_t *) );
    uint64_t **jarrays_to_delete;
    for ( int i = 0 ; i < total_rels ; i++ ) {
      if ( b->already_joined[i] == 1 ) {
        new_jarrays[i] = malloc ( new_size * sizeof(uint64_t) ) ;
      }
    }
    new_jarrays[rel_add] = malloc ( new_size * sizeof(uint64_t) ) ;


    //BEGIN FILLING NEW JARRAYS
    int count = 0;
    uint64_t comp_val,next_val;
    int result_size = result->num_tuples;
    for ( int i = 0 ; i < b->jarrays_size ; i++ ) {

      comp_val = b->jarrays[rel_ref][i];

      if ( similar == 1 ) {
        //CODE
        for ( int s = 0 ; s < ssize ; s++ ) {

            new_jarrays = fill_already_joined_rels ( b , new_jarrays , i , count , total_rels );
            new_jarrays = fill_new_rel ( b , new_jarrays , count , rel_add , new_jarrays[rel_add][count-ssize] );

            count++;
        }

        
        if ( i < b->jarrays_size - 1 ) {
          next_val = b->jarrays[rel_ref][i + 1];
          if ( comp_val == next_val ) {
            similar = 1;
          }
          else {
            similar = 0;
            ssize = 0;
          }
        }
        similar = 0;
        ssize = 0;
      }
      else{
        for ( int r = 0 ; r < result_size ; r++ ) {

          if ( flag == 0 ) {
           if ( comp_val == result->tuples[r].payload) { //IF REL_REF IS THE LEFT VALUE OF THE RESULT ( FLAG == 0 )

             new_jarrays = fill_already_joined_rels ( b , new_jarrays , i , count , total_rels );
             new_jarrays = fill_new_rel ( b , new_jarrays , count , rel_add , result->tuples[r].payload );

             count++;
             ssize++;
            }
          }
          else {
           if ( comp_val == result->tuples[r].key ) { //IF REL_REF IS THE RIGHT VALUE OF THE RESULT ( FLAG == 1 )

            new_jarrays = fill_already_joined_rels ( b , new_jarrays , i , count , total_rels );
            new_jarrays = fill_new_rel ( b , new_jarrays , count , rel_add , result->tuples[r].key );

            count++;
            ssize++;
          }
        }
      }
      if ( i < b->jarrays_size - 1 ) {
         next_val = b->jarrays[rel_ref][i + 1];
         if ( comp_val == next_val ) {
           similar = 1;
         }
         else{
          ssize = 0;
          similar = 0;
         }
      }

    }

    }




    //INSERT NEW JARRAYS TO BETWEEN STRUCT
    jarrays_to_delete = b->jarrays;
    b->jarrays = new_jarrays;
    b->jarrays_size = new_size;


    //DELETE OLD JARRAYS 
    for ( int i = 0 ; i < total_rels ; i++ ){
      if ( b->already_joined[i] == 1 ) {
        free(jarrays_to_delete[i]);
      }
    }
    free(jarrays_to_delete);

    return b;
  }

  uint64_t **fill_already_joined_rels ( Between *b , uint64_t **new_jarrays , int i , int count , int total_rels ) { 
    for ( int rel = 0 ; rel < total_rels ; rel++ ) {
      if ( b->already_joined[rel] == 1 ) {
        new_jarrays[rel][count] = b->jarrays[rel][i];
      }
    }
    return new_jarrays;
  }

  uint64_t **fill_new_rel ( Between *b , uint64_t **new_jarrays , int count , int rel_add , uint64_t value ) {
    new_jarrays[rel_add][count] = value;
    return new_jarrays;
  }



int calculate_new_between_size ( uint64_t *array , relation *result , int bsize , int flag ) {
  int times = 0;
  int total = 0;
  int rsize = result->num_tuples;
  int similar = 0;
  int s_times = 0;
    for ( int i = 0 ; i < bsize ; i++ ) {
      if (similar) {
        total += s_times;
        if ( array[i] == array [i+1])
        {
          similar = 1;
          s_times = times;
        }
        else{
          similar = 0;
          s_times = 0;
        } 
      }
      else
      {
        similar = 0;
        times = 0;
        for ( int j = 0 ; j < rsize ; j++) {
          if (flag == 0) {
            if ( array[i] == result->tuples[j].payload ) { 
              times++;
            }
          }
          else{
            if ( array[i] == result->tuples[j].key ) { 
              times++;
            }
          }
        }
        total += times;
        }
         if ( array[i] == array [i+1])
         {
           similar = 1;
           s_times = times;
         } 
    }
  return total;
}

  Between *between_fill_empty( Between *b , relation *result , int rel1_alias , int rel2_alias ) {

    int size = result->num_tuples;
    b->jarrays[rel1_alias] = malloc ( size * sizeof ( uint64_t ));
    b->jarrays[rel2_alias] = malloc ( size * sizeof ( uint64_t ));
    for ( int i = 0 ; i < size ; i++ ) {
      b->jarrays[rel1_alias][i] = result->tuples[i].payload;
      b->jarrays[rel2_alias][i] = result->tuples[i].key;
    }
    b->jarrays_size = size;
    return b;
  }



relation *prepare_relation ( Between *b , all_data *datatable ,int rel_origin , int rel_alias , int col_no , int flag ) {
  relation *to_return;
  if ( b->already_joined[rel_alias] != 1 ) { //This means this relation hasnt been joined
    if (flag == 1) { //This means that the realtion was filtered
      to_return = build_relation_from_filtered ( b->farrays[rel_alias] , datatable , rel_origin, col_no );
      int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
      bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
      return to_return;
    }//OK
    else { //This means that the relation hasn't been joined nor filtered
      to_return = datatable->table[rel_origin]->columns[col_no];
      int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation
      bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );
      return to_return;
    }
  }//OK
  else { 
    to_return = build_relation_from_joined ( b->jarrays[rel_alias] , b->jarrays_size , datatable , rel_origin , col_no);
    int num_tuples = relation_getnumtuples( to_return ); //total tuples in relation

    bucket_sort ( to_return , 0 , num_tuples - 1 , 1 );

    return to_return;
  }
}


//TO BE FINISHED

relation *build_relation_from_joined ( uint64_t *array , int jsize , all_data *datatable , int rel_origin , int col_no) {
  int i= 0;
  int count = 0;
  
  int num_of_tuples = calc_tuples_size_to_build_rel_from_joined(array, jsize , rel_origin , col_no );

  relation *updated_rel = malloc(sizeof(relation));
  updated_rel->tuples = malloc(sizeof(tuple) * num_of_tuples);
  updated_rel->num_tuples = num_of_tuples;

  while ( i < jsize) {
    if ( i > 0 ) {
      if ( array[i] == array[i-1] ) { //If same with prev don increase
        i++;
      }
      else{ //Not the same with prev
        updated_rel->tuples[count].key = array[i];
        updated_rel->tuples[count].payload = datatable->table[rel_origin]->columns[col_no]->tuples[array[i]].payload;
        count++;
        i++;
      }
    }
    else { // This is the first of the array just incr the coutner
      updated_rel->tuples[count].key = array[i];
      updated_rel->tuples[count].payload = datatable->table[rel_origin]->columns[col_no]->tuples[array[i]].payload;
      i++;
      count++;
    }
  }
  return updated_rel;
}


int calc_tuples_size_to_build_rel_from_joined(uint64_t *array, int jsize , int rel_no , int col_no) 
{
  int count = 0;
  int i = 0;
  while ( i < jsize) {
    if ( i > 0 ) {
      if ( array[i] == array[i-1] ) { //If same with prev don increase
        i++;
      }
      else{ //Not the same with prev
        count++;
        i++;
      }
    }
    else { // This is the first of the array just incr the coutner
      count++;
      i++;
    }
  }
  return count;
}



//FILTERS


int *execute_filter( Predicates * pd , all_data * dt , Predicate * temp_pred , int *result ) 
{

  int rel_no = temp_pred->rel1_origin;
  int rel_size = dt->table[rel_no]->numTuples;

  //Allocate memory in our result array
  result = malloc ( rel_size * sizeof ( int ) );
  for ( int i = 0 ; i < rel_size ; i++ ) 
  {
    result[i] = i;
  }

  char op = temp_pred->op;
  if ( op == '>' )
  {
    greater_filter ( dt , result , temp_pred );
  }
  else if ( op == '<' ) 
  {
    lesser_filter ( dt , result , temp_pred );
  }
  else if (op == '=' )
  {
    equal_filter ( dt , result , temp_pred );
  }

  return result;
}

int *greater_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{
  int rel_no = temp_pred->rel1_origin;
  int col_no = temp_pred->rel1_col;
  uint64_t value =  temp_pred->filter_value;

  uint64_t size = dt->table[rel_no]->numTuples;
  relation *temp_column = dt->table[rel_no]->columns[col_no];


  for ( int i = 0 ; i < size ; i++ )
  {
    if ( temp_column->tuples[i].payload <= value ) 
    {
      result[i] = -1;
    }
  }

  return result;
}

int *lesser_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{

  int rel_no = temp_pred->rel1_origin;
  int col_no = temp_pred->rel1_col;
  uint64_t value =  temp_pred->filter_value;

  uint64_t size = dt->table[rel_no]->numTuples;
  relation *temp_column = dt->table[rel_no]->columns[col_no];


  for ( int i = 0 ; i < size ; i++ ) 
  {
    if ( temp_column->tuples[i].payload > value ) 
    {
      result[i] = -1;
    }
  }

  return result;
}

int *equal_filter ( all_data *dt , int *result , Predicate *temp_pred ) 
{

  int rel_no = temp_pred->rel1_origin;
  int col_no = temp_pred->rel1_col;
  uint64_t value =  temp_pred->filter_value;

  uint64_t size = dt->table[rel_no]->numTuples;
  relation *temp_column = dt->table[rel_no]->columns[col_no];


  for ( int i = 0 ; i < size ; i++ ) 
  {
    if ( temp_column->tuples[i].payload != value ) 
    {
      result[i] = -1;
    }
  }

  return result;
}

int check_if_filtered ( int *filtered , int filter_count , int rel ) 
{ //Return 1 if filtered
  for ( int i = 0 ; i < filter_count ; i++ ) 
  {
    if ( filtered[i] == rel )
    {
      return 1;
    }
  }
  return 0;
}

relation * build_relation_from_filtered(int * array, all_data *dt, int rel_no , int col_no )
{
  uint64_t size = dt->table[rel_no]->numTuples;
  int count= 0;
  
  int num_of_tuples = calc_tuples_size_to_build_rel_from_filtered(array, dt, rel_no , col_no );


  relation *updated_rel = malloc(sizeof(relation));
  updated_rel->tuples = malloc(sizeof(tuple) * num_of_tuples);
  updated_rel->num_tuples = num_of_tuples;
  

  for (int i = 0; i < size ; i++)
  {
    if( array[i] != -1 )
    { 
      updated_rel->tuples[count].key = i;
      updated_rel->tuples[count].payload = dt->table[rel_no]->columns[col_no]->tuples[i].payload;
      count++;

    }
  }
  return updated_rel;
}



int calc_tuples_size_to_build_rel_from_filtered(int *array, all_data *dt, int rel_no , int col_no) 
{
  uint64_t size = dt->table[rel_no]->numTuples;
  int count = 0;

  for (int i = 0; i < size ; ++i)
  {
    if(array[i] != -1)
    {
      count++;
    }
  }
  return count;
}

//OTHERS

int *generate_flags ( int *flags , int *filtered , int filter_count , int rel1 , int rel2 ) {
  flags = malloc ( 2 * sizeof(int) ) ;
  flags[0] = 0;
  flags[1] = 0;
  for ( int i = 0 ; i < filter_count ; i++ ) {
    if ( rel1 == filtered[i] ) {
      flags[0] = 1;
    }
    else if ( rel2 == filtered[i] ) {
      flags[1] = 1;
    }
  }
  return flags;
}

//CONSTRUCTORS


//CONSTRUCTORS


Between *create_between ( Between *this , int total_rels ) //Allocate memory for our between struct
{ 
  this = malloc ( sizeof ( Between ) );
  this = create_arrays ( this , total_rels);
}

Between *create_arrays ( Between *this , int total_rels ) { //Allocate memory for the filtered arrays we are going to create
  this->farrays = malloc ( total_rels * sizeof ( int * ) );
  this->jarrays = malloc ( total_rels * sizeof ( uint64_t * ) );
  this->jarrays_size = 0;
  this->already_joined = malloc ( total_rels * sizeof ( int ));
}


void print_check_sums(Check_sums *cs, all_data *dt,Between *b)
{
  relation *rel_to_print;
  uint64_t sum = 0;
  int result_size = b->jarrays_size;
  int rel_al , rel_or;
  int cur_res;
  int col;

  for (int i = 0; i < cs->size ; i++)
  {
    rel_al = cs->check_sums_array[i].rel_alias;
    rel_or = cs->check_sums_array[i].rel_origin;
    col = cs->check_sums_array[i].rel_col;
    for ( int res = 0 ; res < result_size ; res++ ) {
        cur_res = b->jarrays[rel_al][res];
        sum += dt->table[rel_or]->columns[col]->tuples[cur_res].payload;
    }
    if ( sum == 0 ) {
      printf("NULL\t");
    }
    else {
    printf("%ld\t", sum);
    }
    sum -= sum;
  }
  printf("\n");
}