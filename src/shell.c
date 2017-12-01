#include "input.h"
#include "process.h"
#include "promt.h"

#define MAX 500  //all header file must same.


int main()
{
  char commands[MAX];
  char* argv[MAX];
  while(1)
  {
    promt_write();
    get_input(commands);
    parse(commands, argv);
    process_create(argv);
  }
}
