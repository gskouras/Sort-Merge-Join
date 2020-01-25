#include "../HEADERS/main.h"




// Function bucket_sort used to sort the array

void bucket_sort ( relation * /*relation*/ , int /*start*/ , int /*end*/ , int /*significant byte no*/ ) ;

//Rearrange our relation with help of temp relation

void rearrange ( relation * /*original relation*/ , relation * /*temp relation*/ , int /*start*/ , int /*endbucket_sort*/ , int /*total_tuples*/ , int /*current sig byte position*/ , int ** /* psum */ , int /*psum size*/) ;

//Copy from our temp relation to our original relation starting from start

void copy_from_temp ( relation * , relation * , int , int ) ;

//Search our original relation.For each tuple ,if the key matches with byte then add it to temp.returns temp_counter

int extract_and_add_to_temp ( relation * , relation * , int /*start*/ , int /*end*/ , int /*current temp_counter*/ , int /*current sig byte position*/ , int ) ;

//Calculate size (bytes) of sub-relation

int calculate_size ( int /*total_tuples*/ ) ;

//Create Hist

int *create_hist ( relation * , int * , int , int , int ) ;

//Funtction print_array

void print_array ( int ** /*array*/ , int /*size of array*/ ) ;

//Returns i significant byte of the key

unsigned long long get_sigbyte ( unsigned long long , int ) ;

//Returns requiered size for the psum

int get_psumsize ( int * ) ;

//Fill psum according to given hist

void fill_psum ( int ** , int * , int ) ;

//Count total tuples

int count_tuples ( int , int ) ;

//Quicksort

void quicksort ( relation * , int  , int  ) ;

//Partition

int partition ( relation * , int  , int  ) ;

//Swap tuples in a relationship of the given position

void swap_rel_tuples ( relation * , int , int ) ;

