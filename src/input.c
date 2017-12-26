#include "input.h"
void get_input(char* commands)
{

  enable_raw_mode();

  DIR *e;
  struct dirent *ent;

  int count = 0;
  char dir_file[1000][500];

  int i;
  for (i = 0; i < MAX; i++)
    commands[i] = '\0';

  e = opendir(".");
  if(e != NULL)
  {
    i = 0;
    while((ent = readdir(e)) != NULL)
    {
      strcpy(dir_file[i++], ent->d_name);
    }
    dir_file[i][0] = '\0';
  }
  else
    perror("File can not read!");



  char backspace[] = "\b \b";
  char forward[] = "\x1b[C";
  char back[] = "\x1b[D";
  char frame [5][100];

  strcpy(frame[0], GRN);
  strcpy(frame[1], BLU);
  strcpy(frame[2], MGT);
  strcpy(frame[3], RED);
  strcpy(frame[4], LGT);

  count = 0;
  int cursor_pos = 0;
    int n;
    while(1)
    {
      lseek(STDOUT_FILENO, 0, SEEK_SET);
      fflush(stdout);
      char c = '\0';
      while((n = read(STDIN_FILENO, &c, 1)) != 1)
      {
        if (n == -1 && errno != EAGAIN)
         die("read");

         if(c == '\0') //animation is here...
         {
           int j;
           commands[count] = '\0';

           for(i = 0; i < 5; i++)
           {
             for(j = 0; j < cursor_pos; j++)
              printf("\b");
             printf("\b\b");
             printf("%s> ", frame[i]);

             if(i == 4)
              printf(WHT"%s", commands);
             else
              printf("%s", commands);

             fflush(stdout);
             usleep(100000);
           }
          for(i = 0; i < cursor_pos; i++)
            printf("\b");
          printf("%s\b\b> "WHT, frame[4]);

          for(i = 0; i < cursor_pos; i++)
            printf("\x1b[C");

          fflush(stdout);
        }
      }


      if(c == '\n') // Enter key
      {
          printf("\r\n");
          fflush(stdout);
          break;
      }
      else if(c == 127) // Backspace key
      {
        if(cursor_pos > 0)
        {
          write(STDOUT_FILENO, backspace, 3);
          cursor_pos--;
          count--;
        }
        continue;
      }
      else if(c == '\x1b')
      {
        char arrow_keys[2];
        if(read(STDIN_FILENO, &arrow_keys[0], 1) != 1) continue;
        if(read(STDIN_FILENO, &arrow_keys[1], 1) != 1) continue;

        if(arrow_keys[0] == '[')
          switch (arrow_keys[1]) {
            case 'A':
              printf("[UP]");
              fflush(stdout);
              cursor_pos += 4;
              break;
            case 'B':
              printf("[DOWN]");
              fflush(stdout);
              cursor_pos += 6;
              break;
            case 'C':
              if(cursor_pos < count)
              {
                write(STDOUT_FILENO, forward, 3);
                cursor_pos ++;
              }
              break;
            case 'D':
              if(cursor_pos > 0)
              {
                write(STDOUT_FILENO, back, 3);
                cursor_pos --;
              }
              break;

          }

        continue;
      }
      else if(c == 9) // Tab key
      {
        c = 'b';
        write(STDOUT_FILENO, &c, 1);
        continue;
      }
        cursor_pos++;
        commands[count++] = c;
        printf("%c", c);

    }//while end
      commands[count] = '\0';

      disable_raw_mode();
}

void die(const char* s)
{
  perror(s);
  exit(1);
}

void disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}
void enable_raw_mode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disable_raw_mode);
  struct termios raw = orig_termios;
  // raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN /*| ISIG*/);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 30;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
