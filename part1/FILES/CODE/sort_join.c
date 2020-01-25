#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../HEADERS/sort_join.h"


// Function bucket_sort used to sort the array

void bucket_sort ( relation *rel , int start , int end , int bpos ) {
	if ( bpos <= 8 ){
	/*We check if bpos <=8 for the case when bpos > 8.If bpos >8 that means that all of the keys in the current
		part are equal.This happens because the keys we are checking are 64 bit sized ,
		so if bpos >8 that means that we have examined all 8 bytes and that they are equal.
	*/
		int total_tuples = count_tuples ( start , end ) ;
		int size = calculate_size( total_tuples ) ;
		if ( size <= 1 ) {
			quicksort ( rel , start - 1, end ) ;
		}
		else {
			//Create hist for the part of the relation we are checking
			int *hist = malloc ( 256 * sizeof ( int ) ) ; //Hist is size of 2^8 = 256
			hist = create_hist ( rel , hist , start , end , bpos ) ; //scan the relation and fill the hist
			
			//Check which cells of hist aren't 0 so we can determine the size of psum then fill it according to the given hist
			int psum_size = get_psumsize( hist ); 
			int **psum = malloc ( psum_size * sizeof ( int * ) ) ;
			for (int i = 0 ; i < psum_size ; i++ ){
				psum[i] = malloc (2 * sizeof ( int ) ) ;
			}
			fill_psum ( psum , hist , psum_size );

			//Now rearrange the part that we work in this recursive level with the help of the empty temp relation
			relation *temp;
			temp = relation_create ( temp ) ; //Give space

			rearrange ( rel , temp , start , end , total_tuples , bpos , psum , psum_size ) ;

			//Now for each part of the relation defined by psum we will cal bucket_sort
			if (psum_size > 1) {

				int i = 0;
				int curr_start = start ;
				int curr_end = start + psum[i+1][1] - 1;
				bucket_sort ( rel , curr_start , curr_end , bpos + 1 ) ;
				for ( i = 1 ; i < psum_size - 1 ; i++ ){
					curr_start = curr_end + 1;
					curr_end = curr_start + psum[i+1][1] - psum[i][1] - 1;
					bucket_sort ( rel , curr_start , curr_end , bpos + 1 ) ;
				} 
				int t = curr_start;
				curr_start = curr_end + 1;
				curr_end = end ;
				bucket_sort ( rel , curr_start , curr_end , bpos + 1 ) ;
			}
			else { 
				/* This is the case when psum_size is 1.
				This means that for the current sig byte all tuples are the same so we cant split them in buckets.
				Also this part didnt fit for the quicksort.
				So we examinate the same part with the next siginificant byte.
				*/
				bucket_sort ( rel , start , end , bpos + 1 ) ;
			}

			//Free Memory
			relation_free ( temp ) ;
			free(temp);
			free(hist);
			for ( int k = 0 ; k < psum_size ; k++ ){
				free(psum[k]);
			}
			free(psum);
		}
	}
}

//Rearrange our relation with help of temp relation

void rearrange ( relation *rel , relation *temp , int start , int end , int total_tuples , int bpos , int **psum , int psum_size ) {
	temp = relation_createtuples ( temp , total_tuples ) ; //Create temp's tuples
	int temp_counter = 0; //We keep a counter so we know where we left it from each psum cell
	int curr_sigb = 0; //This is the byte that each psum cell has the frequency for


	for ( int i = 0 ; i < psum_size ; i++ ) { //Fill temp
		curr_sigb = psum[i][0];
		temp_counter = extract_and_add_to_temp ( rel , temp , start , end , bpos , temp_counter , curr_sigb ) ;
	}
	//Now we have temp ready and we copy it to the original relation
	copy_from_temp ( rel , temp , start , temp_counter ) ;
}

//Copy from our temp relation to our original relation starting from start

void copy_from_temp ( relation *rel , relation *temp , int start , int temp_counter ) {
	int copy_counter = 0 ;
	for (int i = start ; i < start + temp_counter ; i++ ){
		unsigned long long key = relation_getkey( temp , copy_counter );
		unsigned long long payload = relation_getpayload( temp , copy_counter );
		//Now copy to rel
		relation_setkey ( rel , i , key );
		relation_setpayload ( rel , i , payload );
		copy_counter++;
	}
}

//Search our original relation.For each tuple ,if the key matches with byte then add it to temp

int extract_and_add_to_temp ( relation *rel , relation *temp , int start , int end , int bpos , int temp_counter , int byte ){
	int r_total_tuples = relation_getnumtuples ( rel );
	unsigned long long key,payload;

	for (int i = start; i < end + 1; i++ ){ //Scan original rel's tuples
		//Get key
		key = relation_getkey ( rel , i /*position of tuple that we want to get*/);
		if ( get_sigbyte ( key , bpos ) == byte ) { //Check if it matches current significant byte
			payload = relation_getpayload ( rel , i ) ; //if yes take payload
			//Then set the values to temp
			relation_setkey( temp, temp_counter , key );
			relation_setpayload( temp , temp_counter , payload );
			temp_counter++;
		}
	}
	return temp_counter;
}

//Fill psum according to given hist

void fill_psum ( int **psum , int *hist , int psum_size ) {
	int i = 0; //idenitifier for psum array
	int carry = 0; //carries the hist value of the last checked so we can add it to the next
	for ( int h = 0 ; h < 256 ; h++ ) {
		if ( hist[h] != 0 ) {
			if ( i == 0 ) { //first cell of psum starts from zero
				psum[i][0] = h;
				psum[i][1] = 0; 
				carry = hist[h] ;
				i++;
			}
			else { //ist cell of psum starts from last cell + last cells carry
				psum[i][0] = h;
				psum[i][1] = psum[i-1][1] + carry;
				carry = hist[h] ;
				i++;
			}
		}
	}
}

//Returns requiered size for the psum

int get_psumsize ( int *hist ) {
	int psum_size = 0;
	for (int i = 0 ; i <= 255 ; i++ ) {
		if ( hist[i] != 0 ) {
			psum_size++;
		}
	}
	return psum_size;
}

//Create Hist

int *create_hist ( relation *rel , int *hist , int start , int end , int bnum ) {
	unsigned long long s_byte;
	unsigned long long key;

	//Set all of hist cells to 0
	for (int h = 0 ; h <= 255 ; h++ ) {
		hist[h] = 0;
	}
	//Check all of the relation's tuples' bnum significant byte and add 1 to the matching hist cell
	for (int i = start ; i <= end ; i++ ) {
		key = relation_getkey ( rel , i ) ;
		s_byte = get_sigbyte ( key , bnum ) ;
		hist[s_byte]++;
	}
	return hist; 
}

//Returns i significant byte of the key

unsigned long long get_sigbyte ( unsigned long long key , int i ) {
	unsigned long long s_byte = key << ( i - 1 ) * 8 ;
	s_byte = s_byte >> 7 * 8 ;
	return s_byte;
}

//Calculate size (bytes) of sub-relation

int calculate_size ( int total_tuples ) {
	int tuple_size = 128 ;
	int total_tuple_size = total_tuples * tuple_size ;
	return total_tuple_size ;
}

//Count total tuples

int count_tuples ( int start , int end ) { 
	/*  EXAMPLE : WHOLE PART <-- 60 tuples
		then start = 0 and end = 59
		so total_tuples = end - start + 1 ;
	*/
	int total_tuples = end - start + 1 ;
	return total_tuples;
}

//Quicksort

void quicksort ( relation *rel , int start , int end ) {
	if ( start < end ) {
        int p = partition ( rel, start , end );
        quicksort( rel , start , p-1 ) ;
        quicksort( rel , p + 1 , end ) ;
    }
}

//Partition

int partition ( relation *rel , int start , int end ) {

    unsigned long long pivot = relation_getkey ( rel , end ) ;  
 
    int i = ( start - 1) ;
    for ( int j = start ; j <= end- 1; j++){
        // If current element is smaller than the pivot
        if ( relation_getkey ( rel , j ) < pivot ){
            i++;
            swap_rel_tuples ( rel , i , j ) ;
        }
    }
    swap_rel_tuples ( rel , (i + 1) , end ) ;
    return (i + 1) ;
}

//Swap tuples in a relationship of the given position

void swap_rel_tuples ( relation *rel , int i , int j ) {
	//Keep j values temp
    unsigned long long temp_key = relation_getkey ( rel , j ) ;
    unsigned long long temp_payload = relation_getpayload ( rel , j ) ;
    //Get i values
    unsigned long long i_key = relation_getkey ( rel , i ) ;
    unsigned long long i_payload = relation_getpayload ( rel , i ) ;
    //Set i-values to j
    relation_setkey ( rel , j , i_key ) ;
    relation_setpayload ( rel , j , i_payload ) ;
    //Set temp values to i
    relation_setkey ( rel , i , temp_key ) ;
    relation_setpayload ( rel , i , temp_payload ) ;
}