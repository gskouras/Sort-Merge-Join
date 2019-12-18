#include "../HEADERS/main.h"


int main()
{ 

  FILE *rel_init = initialize_file();

  all_data *datatable;
  datatable = create_data_table(rel_init);

 	execute_all_batches("small.work", datatable);

  //free_datatable(datatable);

	return 0;
}


all_data* create_data_table( FILE *rel_init)
{
  int c;
  char *filename=NULL;
  size_t linesize;
  all_data *datatable =(all_data *)malloc(sizeof(all_data));
  datatable->num_relations=0;
  datatable->table=NULL;

  getline(&filename,&linesize,rel_init);

  while((c = fgetc(rel_init)) != EOF)
  {
    datatable->table = realloc(datatable->table,sizeof(relation_data *)*(datatable->num_relations+1));
    filename[strlen(filename)-1]='\0';
    datatable->table[datatable->num_relations] = read_data_file(filename);
    datatable->num_relations++;
    getline(&filename,&linesize,rel_init);
  }
  

  printf("\nRelations have been succesfully saved. Please type 'Done' to continue\n\n");
  getline(&filename,&linesize,stdin);

  return datatable;
}


relation_data *read_data_file(char *filename)
{
	uint64_t numofTuples;
	uint64_t numofColumns;
	uint64_t num = 0;


	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("File %s doesn't exist. Please try another\n", filename);
		return NULL;
	}

	fread(&numofTuples, sizeof(uint64_t),1,fp); //read the number of tuples in current file
	fread(&numofColumns,sizeof(uint64_t),1,fp); ///read the numbe of coloumns in current file

	relation_data *reldata =(relation_data *)malloc(sizeof(relation_data)); //allocating memory for relation data(an array of pointers to relations)
	reldata->columns = (relation **)malloc(sizeof(relation *)*numofColumns); //allocating memory for array of pointers which they point to a begging of a coloumn.
	
	for(int i=0;i<numofColumns;i++)
	{
    reldata->columns[i] =(relation *)malloc(sizeof(relation));//allocating memor to store the relations of current file.
    reldata->columns[i]->tuples = (tuple *)malloc(sizeof(tuple)*numofTuples);
    reldata->columns[i]->num_tuples = numofTuples;
	}

  reldata->numColumns = numofColumns;
	//printf("Relations %s has %ju colums", filename,reldata->numColumns);
  reldata->numTuples = numofTuples;
  //printf(" and each column has %ju tuples\n",reldata->numTuples);

  
  //Filing the array of tuples with the elemenets of the file.
  for(int i=0;i<numofColumns;i++)
  {	
    for(int j=0;j<numofTuples;j++)
    {
      	fread(&(num),sizeof(uint64_t),1,fp);
      	reldata->columns[i]->tuples[j].payload=num;
      	reldata->columns[i]->tuples[j].key = j;  
      }
      //printf("total tuples of relation %s columns %d are %d \n", filename, i, reldata->columns[i]->num_tuples);
  }

  return reldata;
}


FILE *initialize_file()
{
  FILE *fp = fopen("small.init", "r");
  if (fp == NULL)
  {
    printf("File small.init doesn't exist. Please try another\n");
    exit(1);
  }
  return fp;
}


// void free_datatable(all_data * dt)
// {
//   for (int i = 0; i < dt->num_relations; i++)
//   { 
//         free(dt->table[i]->columns[i]->tuples);
//   }
//   free(dt->table);
//   free(dt);
// }


