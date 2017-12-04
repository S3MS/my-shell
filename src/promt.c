#include "promt.h"

void promt_write()
{
  char pwd[1024];
  if (getcwd(pwd, sizeof(pwd)) == NULL)
  {
      perror("Current pwd not found.");
      return;
  }
   int count = -1, index = 0, i, j;
   char token[100][100];
   char locate[100];
   char* username = getenv("USER");

  for(i = 0; i < 100; i++)
    for(j = 0; j < 100; j++)
      token[i][j] = '\0';


  for(i = 0; pwd[i] != '\0'; i++)
  {
    if(pwd[i] == '/')
    {
      count++;
      index = 0;
      continue;
    }
    token[count][index++] = pwd[i];
  }

  count = 0;

  if(strcmp(token[0], "home") == 0 && strcmp(token[1], username) == 0)
  {
    locate[0] = '~';
    locate[1] = '/';
    index = 2;
    count = 1;
    for(i = 2; token[i][0] != '\0'; i++)
    {
      for(j = 0; j < 100; j++)
      {
        if(token[i][j] == '\0')
        {
          locate[index++] = '/';
          break;
        }
        else
          locate[index++] = token[i][j];
      }

    }
    locate[--index] = '\0';
  }




  printf(GRN"%s "LGT,username);

  if(count == 1)
    printf("%s ", locate);
  else
    printf("%s ", pwd);

    printf(LGT" > "WHT);

  fflush(stdout);
}
