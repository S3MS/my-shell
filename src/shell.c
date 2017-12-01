#include "input.h"
#include "process.h"

#define MAX 500  //all header file must same.


int main()
{
  char commands[MAX];
  char* argv[MAX];
  while(1)
  {
    get_input(commands);
    parse(commands, argv);
    process_create(argv);
  }
}
