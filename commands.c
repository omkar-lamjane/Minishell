#include <stdio.h>
#include <string.h>
#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","fg" , "bg" , "jobs",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


/*--------------------------------------------------------------------------*/
/*----------------------------for getting command --------------------------*/
/*--------------------------------------------------------------------------*/
char* get_command(char *input_string)
{
    int i = 0;
    if(cmmd == NULL)                // allocate memory once for storing command
        cmmd = malloc(100);

    while(input_string[i] != ' ' && input_string[i] != '\0') // copy characters until space or end of string
    {
        cmmd[i] = input_string[i];
        i++;
    }

    cmmd[i] = '\0';                 // null-terminate the extracted command
    return cmmd;                    // return the command name
}


/*--------------------------------------------------------------------------*/
/*----------------------------for checking command --------------------------*/
/*--------------------------------------------------------------------------*/
int check_command_type(char *command)
{
    for(int i=0; builtins[i] != NULL; i++)       // check if command matches any builtin command
    {
        if(strcmp(command , builtins[i]) == 0)
        {
            return BUILTIN;                      // return BUILTIN if match is found
        }
    }
    
    for(int i=0; ext_cmd[i] != NULL; i++)        // check if command matches any external command
    {
        if(strcmp(command , ext_cmd[i]) == 0)
        {
            return EXTERNAL;                     // return EXTERNAL if match is found
        }
    }
    return NO_COMMAND;                           // return NO_COMMAND if no match is found
}



/*--------------------------------------------------------------------------*/
/*-----------------------for extracting external commands--------------------*/
/*--------------------------------------------------------------------------*/
void extract_external_commands(char **external_commands)
{
    int fd = open("ext_cmd.txt" , O_RDONLY);          // open file containing external commands
    if(fd == -1)
    {
        perror("open");                              // report error if file open fails
        return;
    }

    int count = 0 , i = 0;
    char ch;
    char buffer[100];                                // buffer to store one command

    while(read(fd, &ch , 1) > 0)                     // read file one character at a time
    {
        if(ch == '\n')                               // end of a command line
        {
            buffer[i] = '\0';                        // terminate the command string
            buffer[strcspn(buffer, "\r\n")] = '\0'; // remove carriage return/newline characters
            external_commands[count] = malloc(strlen(buffer) + 1); // allocate memory for command
            if (external_commands[count] == NULL)
                return;                              // return if memory allocation fails
            strcpy(external_commands[count] , buffer); // store command in array
            count++;                                 // move to next command index
            i = 0;                                   // reset buffer index
        }
        else
        {
            buffer[i++] = ch;                        // store character in buffer
        }
    }

    if (i > 0)                                       // handle last command without newline
    {
        buffer[i] = '\0';                            // terminate last command
        external_commands[count] = malloc(strlen(buffer) + 1); // allocate memory
        strcpy(external_commands[count], buffer);    // store last command
    }

    external_commands[count] = NULL;                 // mark end of command list
    close(fd);                                       // close the file
}

						
/*--------------------------------------------------------------------------*/
/*-----------------------for executing internal commands--------------------*/
/*--------------------------------------------------------------------------*/
void execute_internal_commands(char *input_string)
{
    // handle shell termination and clean up resources
    if(strcmp(input_string , "exit") == 0)
    {
        printf("Exiting minishell...\n");
        sleep(1);
        free_external_commands(ext_cmd);     // free external command list
        free_list(head);                     // free job list
        exit(0);                             // terminate the shell
    }

    // print the current working directory
    else if(strcmp(input_string , "pwd") == 0)
    {
        char buf[1024];
        if(getcwd(buf , 1024) != NULL)       // get current directory path
            printf("%s\n",buf);              // display directory
        else
            perror("pwd");                   // report error if getcwd fails
    }

    // change the current working directory
    else if(strncmp(input_string , "cd" , 2) == 0)
    {
        if(chdir(input_string + 3) == 0)     // change directory using given path
        {
            char buf[1024];
            if(getcwd(buf , 1024) != NULL)   // fetch new working directory
                printf("%s\n",buf);          // display updated directory
            else
                perror("pwd");               // report error if getcwd fails
        }
        else
        {
            perror("cd");                    // report error if chdir fails
        }
    }

    // print the current shell process ID
    else if(strncmp(input_string , "echo $$" , 7) == 0)
    {
        printf("%d ",getpid());              // print shell PID
        if(input_string[7] != '\0')          // check for additional text
        {
            printf("%s ",input_string + 7); // print remaining string
            // flag = 1;
        }
        printf("\n");                    // print newline if no extra text
    }

    // print exit status of the last executed command
    else if(strncmp(input_string , "echo $?" , 7) == 0)
    {
        if(WIFEXITED(status))                // check if process exited normally
        {
            printf("%d\n",WEXITSTATUS(status)); // print exit status
        }
        else
        {
            printf("%d\n",WEXITSTATUS(status)); // print status even if abnormal
        }
    }

    // print the path of the current shell
    else if(strncmp(input_string , "echo $SHELL" , 11) == 0)
    {
        printf("%s\n",getenv("SHELL"));      // display SHELL environment variable
    }

    // display the list of stopped/background jobs
    else if(strncmp (input_string , "jobs" , 4) == 0)
    {
        print_list(head);                    // print job list
    }

    // bring the most recent stopped job to foreground and wait
    else if(strncmp(input_string , "fg" , 2) == 0)
    {
        if(head != NULL)                     // check if any job exists
        {
            printf("%s\n",head->cmd);        // display job command
            kill(head->pid , SIGCONT);       // resume stopped process
            waitpid(head->pid , NULL , WUNTRACED); // wait until job completes or stops
            delete_first(&head);             // remove job from list
        }
        else
        {
            printf("-bash: fg: current: no such job\n"); // no job available
        }
    }

    // resume the most recent stopped job in background
    else if(strncmp(input_string , "bg" , 2) == 0)
    {
        if(head != NULL)                     // check if any job exists
        {
            signal(SIGCHLD , signal_handler); // register SIGCHLD handler
            printf("%s &\n",head->cmd);      // show background execution
            kill(head->pid , SIGCONT);       // resume stopped job
            delete_first(&head);             // remove job from list
        }
        else
        {
            printf("-bash: bg: current: no such job\n"); // no job available
        }
    }
}


/*--------------------------------------------------------------------------*/
/*----------------------------for signal handling---------------------------*/
/*--------------------------------------------------------------------------*/
void signal_handler(int sig_num)
{
    if(sig_num == SIGINT && pid == 0)          // handle Ctrl+C when shell is idle
    {
        printf("\n%s ",prompt);               // print prompt on a new line
        fflush(stdout);                       // flush output buffer immediately
    }
    else if(sig_num == SIGTSTP)                // handle Ctrl+Z signal
    {
        if(pid == 0)                           // if shell is not running a child
        {
            printf("\n%s ",prompt);           // reprint prompt
            fflush(stdout);                   // flush output buffer
        }
        else
        {
            insert_at_first(&head);           // store stopped process in job list
        }
    }
    else if(sig_num == SIGCHLD)                // handle termination of child processes
    {
        waitpid(-1 , NULL , WNOHANG);          // reap finished child without blocking
    }
}


/*--------------------------------------------------------------------------*/
/*-----------------------for executing external commands--------------------*/
/*--------------------------------------------------------------------------*/
void execute_external_commands(char *input_string)
{
    // calculate number of arguments and detect presence of pipe
    int row = 1, i = 0, pipe_flag = 0;
    while(input_string[i] != '\0')
    {
        if(input_string[i] == ' ')
            row++;                              // count arguments based on spaces
        else if(input_string[i] == '|')
            pipe_flag = 1;                      // set pipe flag if pipe symbol found
        i++;
    }

    i = 0;
    char *argv[row + 1];                        // argv array for execvp
    char buf[256];                              // temporary buffer for one argument
    int j = 0;
    int index = 0;

    // convert input string into argv array
    while(input_string[i] != '\0')
    {
        if(input_string[i] == ' ')
        {
            buf[j] = '\0';                      // terminate argument
            argv[index] = malloc(strlen(buf) + 1); // allocate memory for argument
            strcpy(argv[index] , buf);          // copy argument
            i++;                                // skip space
            index++;                            // move to next argv index
            j = 0;                              // reset buffer index
        }
        else
        {
            buf[j++] = input_string[i++];       // store character in buffer
        }
    }

    // store the last argument
    if(input_string[i] == '\0')
    {
        buf[j] = '\0';
        argv[index] = malloc(strlen(buf) + 1);  // allocate memory
        strcpy(argv[index] , buf);              // copy last argument
    }

    index++;
    argv[index] = NULL;                         // null-terminate argv array

    // if pipe operator is present
    if(pipe_flag)
    {
        int cmd[index];                         // stores starting index of each command
        int count = 1, pipe_count = 0, i;
        cmd[0] = 0;                             // first command starts at argv[0]

        // identify pipe positions and split argv
        for(i = 0; i < index; i++)
        {
            if(strcmp(argv[i] , "|") == 0)
            {
                argv[i] = NULL;                 // split argv for execvp
                cmd[count++] = i + 1;           // next command starts after pipe
                pipe_count++;                   // increment pipe count
            }
        }

        argv[i] = NULL;                         // mark end of argv
        int fd[2];

        // create processes for each command in pipeline
        for(int i = 0; i <= pipe_count; i++)
        {
            if(i != pipe_count)
            {
                pipe(fd);                       // create pipe
            }

            int pid = fork();                   // fork child process

            if(pid > 0)                         // parent process
            {
                if(i != pipe_count)
                {
                    dup2(fd[0] , 0);            // redirect stdin from pipe read end
                    close(fd[0]);               // close read end
                    close(fd[1]);               // close write end
                }
                waitpid(pid , NULL , 0);        // wait for child process
            }
            else if(pid == 0)                   // child process
            {
                if(i != pipe_count)
                {
                    dup2(fd[1], 1);             // redirect stdout to pipe write end
                    close(fd[0]);               // close unused read end
                }
                execvp(argv[cmd[i]] , argv + cmd[i]); // execute command
                printf("%s: command not found\n", argv[cmd[i]]);
                exit(1);
            }
        }
        exit(0);                                // exit after pipeline execution
    }
    // if no pipe is present
    else
    {
        execvp(argv[0] , argv);                 // execute single external command
        printf("%s: command not found\n", argv[0]);
        exit(1);
    }
}


/*--------------------------------------------------------------------------*/
/*--for deallocate (free) heap memory allocated for external commands (array of pointers)----*/
/*--------------------------------------------------------------------------*/
void free_external_commands(char **external_commands)
{
    for(int i = 0; external_commands[i] != NULL; i++) // iterate through external command list
    {
        free(external_commands[i]);                   // free each allocated command string
    }
}

/*--------------------------------------------------------------------------*/
/*--for deallocate (free) heap memory allocated for linked list-------------*/
/*--------------------------------------------------------------------------*/
void free_list(Slist *head)
{
    Slist *temp;

    while (head != NULL)                               // traverse the linked list
    {
        temp = head;                                  // store current node
        head = head->link;                            // move to next node

        free(temp->cmd);                              // free command string in node
        free(temp);                                   // free node memory
    }
}
