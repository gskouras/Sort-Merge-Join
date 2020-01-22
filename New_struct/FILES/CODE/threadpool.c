#include "../HEADERS/threadpool.h"



void *thread_execute_job ( void *args ) {
    char *result;
    threadpool *thp = (threadpool *) args;
    job_node *job;
    while ( thp->queue->empty == 0 ) {
        pthread_mutex_lock( &thp->get_job );
        //FIRST GET JOB
        job = job_list_get_job (thp->queue);
        //printf("PREDICATE IS %d\n", job->qjob->predicates->predicates_array[0].rel1_origin);
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
    pthread_exit(NULL);
}

void execute_all_batches_threads ( char *filename , all_data * datatable , threadpool *thp ) {
	thp = threadpool_create(thp);
    int total_results;
	FILE *fp = fopen(filename, "r");
	char *line = NULL;
    size_t len = 0;
    int counter = 0;
    int q =0;
 	while(getline(&line, &len, fp) != -1) {
        if ( strcmp ( line , "F\n") == 0 || strcmp ( line , "F" ) == 0 ) {

        //     //GIVE SPACE TO RESULTS ARRAY EQUAL TO THE SIZE OF QUEUE
             total_results = thp->queue->current_jobs;
             thp->results = malloc ( thp->queue->current_jobs * sizeof(char *));

             start_threads ( thp );
             //WAIT FOR ALL THREADS TO FINISH
             join_threads ( thp );
             job_list_print_jobs ( thp->queue );
             printf("BATCH COMPLETE\n");
             for ( int i = 0 ; i < total_results ; i++ ) {
                 printf("%s\n", thp->results[i]);
             }
            counter = 0;

             free(thp->results);
         }
        else {
    	   thp->queue = parse_line( thp->queue , line , datatable , counter );
           counter++;
        }
    }



    //SERIAL
    // job_node *curr_node = thp->queue->root;
    // while (curr_node != NULL) {
    //     execute_query ( curr_node->qjob->predicates, curr_node->qjob->checksums , curr_node->qjob->datatable);
    //     curr_node = curr_node->next;
    //}
    
    threadpool_destroy ( thp );
	// Close the file 
    fclose(fp);
    //}

}


//THREAD FUNCTIONS

void start_threads ( threadpool *thp ) {
    for ( int i = 0 ; i < MAX_THREADS ; i++ ) {
        pthread_create ( &thp->threads[i] , NULL , thread_execute_job , thp ) ;
    }
    pthread_mutex_unlock( &thp->get_job );
}

void join_threads ( threadpool *thp ) {
    for ( int i = 0 ; i < MAX_THREADS ; i++ ) {
        pthread_join ( thp->threads[i] , NULL ) ;
    }
    thp->queue->empty = 0;
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
    if (pthread_mutex_init( &this->thread_join , NULL) != 0) {   printf("\n mutex init failed\n"); }
    if (pthread_mutex_init( &this->thread_create , NULL) != 0) {   printf("\n mutex init failed\n"); }

	return this;
}

void threadpool_destroy ( threadpool *this ) {
    free(this->queue);
    for ( int i = 0 ; i < MAX_THREADS ; i++ ) { 
        pthread_mutex_destroy( &this->get_job );
    }
}