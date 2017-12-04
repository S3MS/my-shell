#include "parse.h"

void parse(char* commands, char* argv[], int* code, char* file[])
{
  char temp[500][500];
  int count = 0, index = 0;
  bool ignore = false;

  *code = 0;
  file[0] = NULL;

  int i, j;
  for(i = 0; i < MAX; i++)
  {
    argv[i] = NULL;
    for(j = 0; j < MAX; j++)
      temp[i][j] = '\0';
  }


  for(i = 0; commands[i] != '\0'; i++)
  {
    if(commands[i] == '\\')
      continue;

    if(commands[i] == '\"' || commands[i] == '\'')
    {
      if(!(i > 0 && commands[i - 1] == '\\'))
      {
        ignore = !ignore;
        continue;
      }
    }


    if(!ignore)
    {
      if(commands[i] != ' ')
        temp[count][index++] = commands[i];
      else
      {
        if(commands[i + 1] != ' ')
        {
          temp[count][index] = '\0';
          count++;
        }
        index = 0;
      }
    }
    else
    {
      temp[count][index++] = commands[i];
    }

    if(commands[i + 1] == '\0')
      temp[i + 1][0] = '\0';
  }

  for(i = 0; temp[i][0] != '\0'; i++)
  {
    if(temp[i][0] == '>')
    {
      *code = 1;
      file[0] = temp[i + 1];
      break;
    }
    else if(temp[i][0] == '<')
    {
      *code = 2;
      file[0] = temp[i + 1];
      break;
    }

    argv[i] = temp[i];
  }

  argv[i] = NULL;


}
