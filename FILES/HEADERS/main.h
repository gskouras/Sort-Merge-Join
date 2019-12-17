#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include "../HEADERS/relation.h"
#include "../HEADERS/batch.h"
#include "../HEADERS/sort_join.h"

relation_data *read_data_file(char * );

all_data * create_data_table( FILE * );

FILE *initialize_file();

void free_datatable(all_data *);



#endif