#include "../HEADERS/threadpool.h"

void execute_all_batches_threads ( char *filename , all_data * datatable , threadpool *thp ) {
	thp = threadpool_create(thp);

	FILE *fp = fopen(filename, "r");
	char *line = NULL;
    size_t len = 0;
 	while(getline(&line, &len, fp) != -1) {
        if ( strcmp ( line , "F\n") == 0 || strcmp ( line , "F") == 0 ) {
            job_list_print_jobs  ( thp->queue );
            printf("BATCH COMPLETE\n");
            //wait
        }
        else {
    	   thp->queue = parse_line( thp->queue , line , datatable );
        }
    }
	// Close the file 
    fclose(fp); 
}


//CONSTRUCTOR

threadpool *threadpool_create ( threadpool *this ) {
	this = malloc(sizeof(threadpool));
	this->queue = job_list_create (this->queue);
	return this;
}