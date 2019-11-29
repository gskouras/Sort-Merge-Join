#include "../HEADERS/predicates.h"


int calc_predicates_num(char *query)
{
	int predicates_num = 0;

	for (int i = 0; i < strlen(query) - 3; ++i)
	{
		if (query[i] == '&')
			predicates_num++;
	}
	return ++predicates_num;
}


predicates **fill_predicates(char *token , int num_predicates)
{

}


void print_predicates (predicates ** predicates,int num_predicates)
{
  int i;
  for (i=0;i<num_predicates;i++)
    {
      if (predicates[i]->rel1!=-1 && predicates[i]->rel2!=-1)
      printf("%d.%d%c%d.%d\n",predicates[i]->rel1,predicates[i]->col1,predicates[i]->op,predicates[i]->rel2,predicates[i]->col2);
      else if (predicates[i]->rel1==-1)
      printf("%d%c%d.%d\n",predicates[i]->col1,predicates[i]->op,predicates[i]->rel2,predicates[i]->col2);
      else if (predicates[i]->rel2==-1)
      printf("%d.%d%c%d\n",predicates[i]->rel1,predicates[i]->col1,predicates[i]->op,predicates[i]->col2);
    }
}