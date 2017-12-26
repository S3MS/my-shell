#include "promt.h"



void promt_write()
{
  int count = -1, index = 0, i, j;
  char token[100][100];
  char locate[100];
  char* username = getenv("USER");
  char pwd[1024];

  if (getcwd(pwd, sizeof(pwd)) == NULL)
  {
      perror("Current pwd not found.");
      return;
  }


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

  for(i = 0; username[i] != '\0'; i++);

  G_LENGHT = 0;
  G_LENGHT += i;


    strcpy(G_USERNAME, username);


  printf(GRN"%s "BLU,username);

  if(count == 1)
  {
    printf("%s ", locate);
    strcpy(G_LOCATE, locate);

    for(i = 0; locate[i] != '\0'; i++);
    G_LENGHT += i;
    if(i == 1)
      G_LENGHT += 2;
  }

  else
  {
    printf("%s ", pwd);
    strcpy(G_LOCATE, pwd);

    for(i = 0; pwd[i] != '\0'; i++);
    G_LENGHT += i + 2;
  }

    printf(LGT" > "WHT);

    G_LENGHT += 3;

  fflush(stdout);
}
