#ifndef _JSON_H
#define _JSON_H
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash.h"
#include "cJSON.h"

typedef struct
{
    int speed;
    double weight;
    int queue_num;
} port_info;

// parse a struct array
// parse a struct array
int cJSON_to_struct_array(char *text, port_info worker[]);

// Read a file, parse, render back, etc.
void dofile(char *filename);

#endif