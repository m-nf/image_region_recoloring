#ifndef ccl
#define ccl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENTS_COUNT 3
#define MAX_HEADER_ROW_LENGTH 20
#define NUMBER_OF_NEIGHBOURS 4
#define NUMBER_OF_SHADES 256

typedef unsigned char uchar;

typedef struct themyfile {
    FILE *fp;
    uchar *data, equivalency[NUMBER_OF_SHADES], neighbours_values[NUMBER_OF_NEIGHBOURS];
    int columns, rows, data_length, shades_labels[NUMBER_OF_SHADES];
} myfile;

#endif