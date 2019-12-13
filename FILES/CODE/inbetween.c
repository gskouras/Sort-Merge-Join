#include "../HEADERS/inbetween.h"

void execute_filters(Predicates *pd, all_data *dt)
{
	int count = 0;
	for (int i = 0; i < pd->size; ++i)
	{
		if(pd->predicates_array[i].rel2_alias == -1)
		{
			if(pd->predicates_array[i].op == '>')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload > 
					pd->predicates_array[i].filter_value)
					{
						count++;
						//insert_to_inbet_array()
					}
				}
				printf("Total tuples where %d\n", count);
			}else if(pd->predicates_array[i].op == '<')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload < 
					pd->predicates_array[i].filter_value)
					{
						count++;
						//insert_to_inbet_array()
					}

				}
				printf("Total tuples where %d\n", count);
			}
			else if(pd->predicates_array[i].op == '=')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload ==
					pd->predicates_array[i].filter_value)
					{
						count++;
						//insert_to_inbet_array()
					}
				}
			printf("Total tuples where %d\n", count);	
			}
		}
	}
}