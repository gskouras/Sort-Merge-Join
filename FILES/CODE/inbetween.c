#include "../HEADERS/inbetween.h"


void execute_predicates(Predicates *pd, all_data *datatable)
{
	relation *updated_rel = (relation*)malloc(sizeof(relation));
	Inbetween_results inbet_res;
	inbet_res.inbet_res_array = malloc(sizeof(Inbet_result) * pd->size);
	inbet_res.size = pd->size;
	inbet_res.current = 0;

	inbet_res.inbet_res_array = malloc(sizeof(Inbet_result) * pd->size);
	for (int i = 0; i < pd->size; ++i)
	{
		if(pd->predicates_array[i].rel2_alias == -1)
		{	
			int num_of_tuples = calc_tuples_size_after_filter(pd, datatable);
			updated_rel->tuples = malloc(sizeof(tuple)*num_of_tuples);
			updated_rel->num_tuples = 0;
			updated_rel = execute_filters(pd, datatable, updated_rel, num_of_tuples);
			
			inbet_res.inbet_res_array[inbet_res.current].rel = updated_rel;
			inbet_res.inbet_res_array[inbet_res.current].rel1_origin = pd->predicates_array[i].rel1_origin;
			inbet_res.inbet_res_array[inbet_res.current++].rel1_alias = pd->predicates_array[i].rel1_alias;
		}

	}
	//relation_print(updated_rel);
	//printf("Original updated relation is %d and its alias is %d \n",inbet_res.inbet_res_array[0].rel1_origin, inbet_res.inbet_res_array[0].rel1_alias);
}





















int calc_tuples_size_after_filter(Predicates *pd, all_data *dt)
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
					}
				}
			}else if(pd->predicates_array[i].op == '<')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload < 
					pd->predicates_array[i].filter_value)
					{
						count++;
					}

				}
			}
			else if(pd->predicates_array[i].op == '=')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload ==
					pd->predicates_array[i].filter_value)
					{
						count++;
					}
				}	
			}
		}
	}
	return count;
}


relation *execute_filters(Predicates *pd, all_data *dt, relation* updated_rel, int num_of_tuples)
{
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
						//printf("position is %d\t", position);
						updated_rel->tuples[updated_rel->num_tuples].key = updated_rel->num_tuples;
						updated_rel->tuples[updated_rel->num_tuples++].payload = dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload;
					}
				}
			}else if(pd->predicates_array[i].op == '<')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload < 
					pd->predicates_array[i].filter_value)
					{	
						updated_rel->tuples[updated_rel->num_tuples].key = updated_rel->num_tuples;
						updated_rel->tuples[updated_rel->num_tuples++].payload = dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload;
					}

				}
			}
			else if(pd->predicates_array[i].op == '=')
			{
				for(int j = 0; j < dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->num_tuples; j++)
				{
					if(dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload ==
					pd->predicates_array[i].filter_value)
					{
						updated_rel->tuples[updated_rel->num_tuples].key = updated_rel->num_tuples;
						updated_rel->tuples[updated_rel->num_tuples++].payload = dt->table[pd->predicates_array[i].rel1_origin]->columns[pd->predicates_array[i].rel1_col]->tuples[j].payload;
					}
				}	
			}
		}
	}
	//relation_print(updated_rel);
	return updated_rel;
}