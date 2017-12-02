#include "input.h"
void get_input(char* commands)
{
  int count = 0;
  int i;
  for (i = 0; i < MAX; i++)
    commands[i] = '\0';

  while(1)
  {
    char c = fgetc(stdin);
    if(c == '\n')
      break;

    commands[count++] = c;

  }

  commands[count] = '\0';
}
