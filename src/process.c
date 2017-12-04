#include "process.h"

void process_create(char* argv[], int* code, char* file[])
{
  if(argv[0] == NULL)
    return;

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

    int fd;

    if(*code == 1)
    {
      if ((fd = open(file[0], O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)) < 0)  {
       perror("Err ");
       exit(EXIT_FAILURE);
      }
      dup2(fd, 1);
    }
    if(*code == 2)
    {
      if ((fd = open(file[0], O_RDONLY)) < 0)  {
       perror("Err ");
       exit(EXIT_FAILURE);
      }
      dup2(fd, 0);
    }

    if(*code != 0)
      close(fd);

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
