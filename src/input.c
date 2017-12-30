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

              for(j = 0; j < count - cursor_pos; j++)
                printf("\b");

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

          for(i = cursor_pos - 1; i < count; i++)
            commands[i] = commands[i + 1];

          commands[count - 1] = ' ';

          for(i = cursor_pos - 1; i < count; i++)
              printf("%c", commands[i]);



          for(i = cursor_pos - 1; i < count; i++)
            printf("\b");
          cursor_pos--;
          count--;
        }
        continue;
      }
      else if(c == '\x1b')
      {
        char* history;
        char arrow_keys[2];
        if(read(STDIN_FILENO, &arrow_keys[0], 1) != 1) continue;
        if(read(STDIN_FILENO, &arrow_keys[1], 1) != 1) continue;

        if(arrow_keys[0] == '[')
          switch (arrow_keys[1]) {
            case 'A':
              if((history = get_forward_history()) != '\0')
              {
                char temp[500];
                strcpy(temp, history);
                int k;
                for(k = 0; k < count; k++)
                  write(STDOUT_FILENO, backspace, 3);
                for(k = 0; temp[k] != '\0'; k++)
                  commands[k] = temp[k];
                cursor_pos = k;
                count = k;
                printf("%s", history);
              }
              break;
            case 'B':
              if((history = get_back_history()) != '\0')
              {
                char temp[500];
                strcpy(temp, history);
                int k;
                for(k = 0; k < count; k++)
                  write(STDOUT_FILENO, backspace, 3);
                for(k = 0; temp[k] != '\0'; k++)
                  commands[k] = temp[k];
                cursor_pos = k;
                count = k;
                printf("%s", history);
              }
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
        if(cursor_pos - 1 == count)
        {
            commands[count++] = c;
            printf("%c", c);
        }

        else
        {
          for(i = count -1; i >= cursor_pos - 1; i--)
          {
            commands[i + 1] = commands[i];
          }
          commands[cursor_pos - 1] = c;
          count++;

          for(i = cursor_pos - 1; i < count; i++)
            printf("%c", commands[i]);

          for(i = cursor_pos; i < count; i++)
            printf("\b");


        }


    }//while end
      commands[count] = '\0';
      if(count != 0)
      {
        add_history(commands);
        empty_buffer();
      }

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

    //for now good here!
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
