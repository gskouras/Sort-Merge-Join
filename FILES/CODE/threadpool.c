#include "../HEADERS/threadpool.h"

void execute_all_batches_threads ( char *filename , all_data * datatable , threadpool *thp ) {
	thp = threadpool_create(thp);
    int total_results;
	FILE *fp = fopen(filename, "r");
	char *line = NULL;
    size_t len = 0;
    int counter = 0;
 	while(getline(&line, &len, fp) != -1) {
        if ( strcmp ( line , "F\n") == 0 || strcmp ( line , "F") == 0 ) {
            //GIVE SPACE TO RESULTS ARRAY EQUAL TO THE SIZE OF QUEUE
            total_results = thp->queue->current_jobs;
            thp->results = malloc ( thp->queue->current_jobs * sizeof(char *));
            start_threads ( thp );
            //WAIT FOR ALL THREADS TO FINISH
            join_threads ( thp );
            printf("JOBS NOW %d\n", thp->queue->current_jobs);
            printf("BATCH COMPLETE\n");
            for ( int i = 0 ; i < total_results ; i++ ) {
                printf("%s\n", thp->results[i]);
            }
            counter = 0;
        }
        else {
    	   thp->queue = parse_line( thp->queue , line , datatable , counter );
           counter++;
        }
    }
	// Close the file 
    fclose(fp); 
}


//THREAD FUNCTIONS


void *thread_execute_job ( void *args ) {
    char *result;
    threadpool *thp = (threadpool *) args;
    job_node *job;
    pthread_mutex_lock( &thp->get_job );
    while ( thp->queue->empty == 0 ) {
        //FIRST GET JOB
        job = job_list_get_job (thp->queue);
        pthread_mutex_unlock( &thp->get_job );

        if ( job != NULL ){
            //CRITICAL SECTION

               if ( job->type == 0 ) { //This means it is a query job
                result = execute_query ( job->qjob->predicates , job->qjob->checksums , job->qjob->datatable );
                thp->results[job->qjob->id] = malloc ( strlen ( result ) + 1);
                strcpy(thp->results[job->qjob->id] , result);
                //REMOVE JOB
                pthread_mutex_lock( &thp->get_job );
                job_list_remove_job ( thp->queue , job->qjob->id , job->type);
                pthread_mutex_unlock( &thp->get_job );
            }
        }
    }
    return NULL;
}





void start_threads ( threadpool *thp ) {
    for ( int i = 0 ; i < MAX_THREADS ; i++ ) {
        pthread_create ( &thp->threads[i] , NULL , thread_execute_job , thp ) ;
    }
}

void join_threads ( threadpool *thp ) {
    for ( int i = 0 ; i < MAX_THREADS ; i++ ) {
        pthread_join ( thp->threads[i] , NULL ) ;
    }
}

void *thread_test ( void *args ) {
    printf("THREAD PRINTED\n");
}




//CONSTRUCTOR

threadpool *threadpool_create ( threadpool *this ) {
	this = malloc(sizeof(threadpool));
    //FIRST CREATE QUEUE
	this->queue = job_list_create (this->queue);
    //THEN INIT MUTEXES

    if (pthread_mutex_init( &this->get_job , NULL) != 0) {   printf("\n mutex init failed\n"); }

	return this;
}
