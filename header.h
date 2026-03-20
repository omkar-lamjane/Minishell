#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct Slist
{
    int pid;
    char *cmd;
    struct Slist *link;
}Slist;

extern char *cmmd;
extern char *ext_cmd[155];
extern char *builtins[];
extern char prompt[25];
extern int pid , status;
extern Slist *head;
extern char input_string[50];


void scan_input(char *prompt, char *input_string);
char* get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

void free_external_commands(char **external_commands);
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void execute_external_commands(char *input_string);


void insert_at_first(Slist **head);
void print_list(Slist *head);
void delete_first(Slist **head);
void free_list(Slist *head);

#endif