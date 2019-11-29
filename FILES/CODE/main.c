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
    datatable->table[datatable->num_relations] = read_file(filename);
    datatable->num_relations++;
    getline(&filename,&linesize,stdin);
  }

  // uint64_t element = datatable->table[1]->columns[2]->tuples[37].payload;
  // printf("The value of element is %ld\n",element);

 	printf("Now please enter a batch of queries to exexute. When the batch is over press F\n");
 	while (getline(&filename, &linesize, stdin))
 	{

 	}
	return 0;
}


relation_data *read_file(char *filename)
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
	printf("Has %ju colums\n\n",reldata->numColumns);
  reldata->numTuples = numofTuples;
  printf("Has %ju tuples\n\n",reldata->numTuples);

  //Filing the array of tuples with the elemenets of the file.
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

