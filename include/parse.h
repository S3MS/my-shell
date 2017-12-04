#ifndef PARSE_H
#define PARSE_H

#include "stdio.h"
#include "stdlib.h"

#define MAX 500  //all header file must same.
typedef enum
{
  false = 0,
  true = 1
}bool;

void parse(char*, char* argv[], int*, char* file[]);

#endif
