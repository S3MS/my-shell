#include "process.h"

void process_create(char* argv[])
{
  int pid = fork();

  if(pid > 0)
  {
    //parent
    wait(NULL);


  }
  else if(pid == 0)
  {
    //child
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
