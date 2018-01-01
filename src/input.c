#include "input.h"
void get_input(char* commands)
{

  enable_raw_mode();

  int count = 0;
  int dir_index[10000];
  char dir_file[10000][500];

  int i;
  for (i = 0; i < MAX; i++)
    commands[i] = '\0';


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
  int tab_completion_count = 0;
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
           cursor_pos -= dif_tr(commands);

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

              for(j = 0; j < count - cursor_pos - dif_tr(commands); j++)
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
          cursor_pos += dif_tr(commands);
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
          tab_completion_count = 0;
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
        tab_completion_count++;
        tab_completion(dir_file);
        int t, r, value, count_index = 0;
        char temp_back[500];
        int check_index = -1;
        for(r = cursor_pos - 1; r >= 0; r--)
        {
          if(commands[r] == ' ')
          {
            check_index = r;
            break;
          }

        }
        for(r = 0; r <= check_index; r++)
          temp_back[r] = commands[r];

          temp_back[r] = '\0';

          for(r = 0; dir_file[r][0] != '\0'; r++)
          {
            for(t = check_index + 1; t < cursor_pos; t++)
            {
              value = 1;
              if(dir_file[r][t - check_index - 1] != commands[t])
              {
                value = 0;
                break;
              }
            }
            if(value)
              dir_index[count_index++] = r;
          }
          if(count_index == 1)
          {
            char temp_forward[500];
            int t_index = 0;
            r = cursor_pos;
            for(r = cursor_pos; r < count; r++)
              temp_forward[r - cursor_pos] =  commands[r];
            temp_forward[r - cursor_pos] = '\0';
            t_index = r - cursor_pos;

            for(t = 0; dir_file[dir_index[0]][t] != '\0'; t++)
              commands[t] = dir_file[dir_index[0]][t];

            commands[t] = '\0';

            strcat(commands, temp_forward);

            for(r = 0; r < count - cursor_pos; r++)
              write(STDOUT_FILENO, forward, 3);


            for(r = 0; r < count; r++)
              write(STDOUT_FILENO, backspace, 3);


            strcat(temp_back, commands);
            strcpy(commands, temp_back);
            printf("%s", commands);

            count = t + t_index + check_index + 1;
            cursor_pos = t + check_index + 1;
            tab_completion_count = 0;


            for(r = 0; r < count - cursor_pos; r++)
              printf("\b");

            fflush(stdout);
          }
          else if(count_index > 0 && tab_completion_count == 2)
          {
            printf("\r\n");
            for(t = 0; t < count_index; t++)
              printf("%s  ", dir_file[dir_index[t]]);
            printf("\n");
            promt_write();
            commands[count] = '\0';
            printf("%s", commands);

            for(t = 0; t < count - cursor_pos; t++)
              printf("\b");

            fflush(stdout);
            tab_completion_count = 0;
          }
        continue;
      }
        cursor_pos++;
        tab_completion_count = 0;
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
      }

      disable_raw_mode();
}

int dif_tr(char* command)
{
  int i, j;
  for(i = 0; command[i] != '\0'; i++);
  int ret = i;
  int dif = 0;
  char* alphabet = "üğşçöıÜĞŞÖÇİ";

  // char alphabet[50];
  //
  // for(i = 0; i < 25; i++)
  // {
  //   alphabet[i] = i + 65;
  //   alphabet[i + 25] = i + 97;
  // }

  for(i = 0; command[i] != '\0'; i++)
  {
    for(j = 0; j < 12; j++)
      if(command[i] == alphabet[j])
      {
        dif++;
        break;
      }
  }
  return (dif / 2);
}

void tab_completion(char dir_file[][500])
{
  DIR *e, *sys;
  struct dirent *ent;
  e = opendir(".");
  sys = opendir("/usr/bin/");
  if(e != NULL)
  {
    int i = 0;
    while((ent = readdir(e)) != NULL)
    {
      strcpy(dir_file[i++], ent->d_name);
    }
    while((ent = readdir(sys)) != NULL)
    {
      strcpy(dir_file[i++], ent->d_name);
    }
    dir_file[i][0] = '\0';
  }
  else
    perror("File can not read!");
}

void die(const char* s)
{
  perror(s);
  exit(1);
}

void disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");

  empty_buffer(); //for now good here!
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
