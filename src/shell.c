#include "input.h"
#include "process.h"
#include "promt.h"

#define MAX 500  //all header file must same.


int main()
{
  char commands[MAX];
  char* argv[MAX];
  int code;
  char* file[1];

  while(1)
  {
    promt_write();
    get_input(commands);
    parse(commands, argv, &code, file);
    process_create(argv, &code, file);
  }
}
