#ifndef HISTORY_H
#define HISTORY_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"

static int LAST_INDEX = 0;
static int CURRENT_INDEX = 0;
static int BUFFER_INDEX = 0;
static int START_INDEX = 0;
char read_line[500][500];

void init_history();
char* get_forward_history();
char* get_back_history();
void add_history(char*);
void empty_buffer();



#endif
