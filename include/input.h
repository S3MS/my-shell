#ifndef INPUT_H
#define INPUT_H

#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "dirent.h"
#include "string.h"
#include "unistd.h"
#include "ncurses.h"
#include <termios.h>
#include "promt.h"

struct termios orig_termios;

#define MAX 500  //all header file must same.

void get_input(char*);

void enable_raw_mode();
void disable_raw_mode();
void die(const char*);
#endif
