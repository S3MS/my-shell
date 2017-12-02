#ifndef PROCESS_H
#define PROCESS_H

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"

void process_create(char* argv[]);
#endif
