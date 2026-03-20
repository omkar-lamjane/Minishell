#include "header.h"
#include <string.h>



void scan_input(char *prompt , char *input_string)
{
    signal(SIGINT , signal_handler);        // register custom handler for Ctrl+C
    signal(SIGTSTP , signal_handler);       // register custom handler for Ctrl+Z

    while (1)                               // shell runs continuously
    {
        pid = 0;                            // reset pid to indicate no foreground process
        printf("%s ",prompt);               // display shell prompt

        scanf("%[^\n]",input_string);       // read input until newline
        getchar();                          // consume newline character

        // handle prompt change using PS1
        if(strncmp(input_string , "PS1=" , 4) == 0)
        {
            int len = strlen(input_string);
            int flag = 1;
            char *fail;

            for(int i = 4; i < len - 1; i++) // validate PS1 assignment
            {
                if(input_string[i] == ' ')
                {
                    input_string[i] = '\0';
                    if((input_string[i + 1] != ' '))
                    {
                        fail = input_string + (i+1);
                        flag = 0;
                        break;
                    }
                }
            }

            if(flag)
            {
                strcpy(prompt , input_string + 4); // update shell prompt
            }
            else
            {
                printf("%s: command not found\n",fail); // invalid PS1 usage
            }
        }
        else
        { 
            cmmd = get_command(input_string);   // extract command name from input

            int type = check_command_type(cmmd); // determine command type

            // execute based on command type
            if(type == BUILTIN)
            {
                execute_internal_commands(input_string); // execute built-in command
            }
            else if(type == EXTERNAL)
            {
                pid = fork();                    // create child process
                if(pid > 0)
                {
                    waitpid(pid , &status , WUNTRACED); // wait for child to finish or stop
                }
                else if(pid == 0)
                {
                    signal(SIGINT , SIG_DFL);    // restore default Ctrl+C handling in child
                    signal(SIGTSTP , SIG_DFL);   // restore default Ctrl+Z handling in child
                    execute_external_commands(input_string); // execute external command
                }
            }
            else
            {
                printf("%s: command not found\n",input_string); // invalid command
            }
        }
    }
}
