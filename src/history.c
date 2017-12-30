#include "history.h"
void init_history()
{
  int count = 0;
  int index = 0;
  const char *home = getenv("HOME");
  char destination[500];

  strcpy(destination, home);
  strcat(destination, "/.my-shell_history");

  int fp;
  if((fp = open(destination, O_RDWR)) < 0)
  {
    perror("History ");
    exit(EXIT_FAILURE);
  }
  read(fp, &read_line[0][0],1);
    while(read(fp, &read_line[count][++index], 1))
    {
      if(read_line[count][index] == '\n')
      {
        read_line[count][index] = '\0';
        if(count > 0 && strcmp(read_line[count], read_line[count - 1]) != 0)
          count++;
        if(count == 0)
          count++;
        index = -1;
      }
    }
      read_line[count][0] = '\0';

      LAST_INDEX = count - 1;
      CURRENT_INDEX = LAST_INDEX + 1;
      BUFFER_INDEX = LAST_INDEX + 1;
      START_INDEX = LAST_INDEX + 1;
      close(fp);
}
char* get_forward_history()
{
  char* ret;
  if(CURRENT_INDEX > 0)
    ret = read_line[--CURRENT_INDEX];
  else
    ret = read_line[CURRENT_INDEX];

  return ret;


}
char* get_back_history()
{
  char* ret;
  if(CURRENT_INDEX <= LAST_INDEX)
    ret  = read_line[++CURRENT_INDEX];
  else
    ret = read_line[CURRENT_INDEX];

  return ret;
}

void add_history(char* command)
{
  strcpy(read_line[BUFFER_INDEX++], command);
  LAST_INDEX++;
  CURRENT_INDEX = LAST_INDEX + 1;
}

void empty_buffer()
{
  char destination[500];
  const char *home = getenv("HOME");
  strcpy(destination, home);
  strcat(destination, "/.my-shell_history");
  FILE* fp = fopen(destination,"a");
  int i;
  for(i = START_INDEX; i <= LAST_INDEX; i++)
      fprintf(fp, "%s\n", read_line[i]);

  START_INDEX = LAST_INDEX + 1;
  fclose(fp);
}
