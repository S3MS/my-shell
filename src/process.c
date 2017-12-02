#include "process.h"

void process_create(char* argv[])
{
  int is_cd = 0;
  if(strcmp(argv[0],"cd") == 0)
  {
    is_cd = 1;
    int result;
    const char* home = getenv("HOME");

    if(argv[1] != NULL)
       result = chdir(argv[1]);
    else
       result = chdir(home);
       
    if(result != 0)
    {
      switch(result)
      {
        case EACCES: perror("Permission denied");
          break;
        case EIO:	 perror("An input output error occured");
          break;
        case ENAMETOOLONG: perror("Path is to long");
          break;
        case ENOTDIR: perror("A component of path not a directory");
          break;
        case ENOENT: perror("No such file or directory");
          break;
        default: perror("Couldn't change directory");
      }
    }
  }

  int pid = fork();

  if(pid > 0)
  {
    //parent
    wait(NULL);


  }
  else if(pid == 0)
  {
    //child
    if(is_cd)
      exit(0);
    if(execvp(argv[0],argv) < 0)
    {
      perror("Err ");
      exit(EXIT_FAILURE);
    }

    fflush(stdout);

    exit(0);
  }
  else
  {
    //fork error
    perror("Fork Failed.");
    exit(EXIT_FAILURE);
  }
}
