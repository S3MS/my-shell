#include "parse.h"

void parse(char* commands, char* argv[])
{
  char temp[500][500];
  int count = 0, index = 0;
  int i, j;
  for(i = 0; i < MAX; i++)
  {
    argv[i] = NULL;
    for(j = 0; j < MAX; j++)
      temp[i][j] = '\0';
  }




  for(i = 0; commands[i] != '\0'; i++)
  {
    if(commands[i] != ' ')
      temp[count][index++] = commands[i];
    else
    {
      if(commands[i + 1] != ' ')
        count++;
      index = 0;
    }
    if(commands[i + 1] == '\0')
      temp[i + 1][0] = '\0';
  }
  for(i = 0; temp[i][0] != '\0'; i++)
    argv[i] = temp[i];

  argv[i] = NULL;


}
