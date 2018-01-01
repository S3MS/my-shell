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
#include "history.h"


struct termios orig_termios;

#define MAX 500  //all header file must same.

void get_input(char*);
void tab_completion(char [][500]);
void enable_raw_mode();
void disable_raw_mode();
void die(const char*);

int dif_tr(char*);

#endif
