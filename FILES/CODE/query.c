#include "../HEADERS/query.h"

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


