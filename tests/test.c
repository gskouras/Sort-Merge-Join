#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>



typedef struct
{
	char **batch;
	int size;
} Batch_lines;


typedef struct
{
	Batch_lines *batches;
	int size;
} Batches;

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
			printf("%s",batches->batches[i].batch[k]);
		}
		getline(&line, &len, fp); // einai to F
		printf("%s", line);
		if(i == batches->size-1)
			continue;
		token = strtok(NULL, "_");
		count = atoi(token);
	}


	fclose(fp);

	if (line)
        free(line);
}

void freeAll(Batches * batches){
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

	// valgrind ./test --leak-check=full --show-leak-kinds=all


int main(void){

	char str[100];
	Batches *batches = createBatches("small.work", str);
	printf("%d\n", batches->size);
	printf("%s\n",str);
	fillBatches(batches, str, "small.work");


	freeAll(batches);



	return 0;



}