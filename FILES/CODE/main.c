#include "../HEADERS/main.h"


int main()
{
	char *filename=NULL;
  	size_t linesize;

  	all_data *datatable =(all_data *)malloc(sizeof(all_data));
 	datatable->num_relations=0;
  	datatable->table=NULL;
 

  	printf("\nEnter the filenames which contains the relation. When you finish type: Done\n");
  	getline(&filename,&linesize,stdin);

  	while(strcmp(filename,"Done\n")!= 0)
  	{
    	datatable->table = realloc(datatable->table,sizeof(relation_data *)*(datatable->num_relations+1));
    	filename[strlen(filename)-1]='\0';
    	datatable->table[datatable->num_relations] = parsefile(filename);
    	datatable->num_relations++;
    	getline(&filename,&linesize,stdin);
  }

   uint64_t element = datatable->table[1]->columns[2]->tuples[37].payload;
   printf("The value of element is %ld\n",element);


	return 0;
}


relation_data *parsefile(char *filename)
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

	fread(&numofTuples, sizeof(uint64_t),1,fp);
	fread(&numofColumns,sizeof(uint64_t),1,fp);

	relation_data *reldata =(relation_data *)malloc(sizeof(relation_data));
	reldata->columns = (relation **)malloc(sizeof(relation *)*numofColumns);
	
	for(int i=0;i<numofColumns;i++)
	{
    reldata->columns[i] =(relation *)malloc(sizeof(relation));
    reldata->columns[i]->tuples = (tuple *)malloc(sizeof(tuple)*numofTuples);
    reldata->columns[i]->num_tuples = numofTuples;
	}

  	reldata->numColumns = numofColumns;
	printf("Has %ju colums\n\n",reldata->numColumns);
  	reldata->numTuples = numofTuples;
  	printf("Has %ju tuples\n\n",reldata->numTuples);

  	for(int i=0;i<numofColumns;i++)
  	{	
    	for(int j=0;j<numofTuples;j++)
    	{
      		fread(&(num),sizeof(uint64_t),1,fp);
      		reldata->columns[i]->tuples[j].payload=num;
      		reldata->columns[i]->tuples[j].key = j;
      	}
    }

    return reldata;
}

