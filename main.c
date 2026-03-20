/*
 * MiniShell
 * ---------
 * This program implements a basic Unix-like shell in C. The minishell supports
 * execution of built-in commands, external commands, pipelines, and
 * basic job control similar to a standard Linux shell.
 *
 * Features:
 * ----------
 * 1. Command Execution
 *    - Executes external commands using fork(), execvp(), and waitpid().
 *    - Supports pipelines using the pipe (|) operator.
 *
 * 2. Built-in Commands
 *    - pwd            : Prints the current working directory.
 *    - cd <dir>       : Changes the current working directory.
 *    - exit           : Terminates the shell.
 *    - echo $?        : Prints the exit status of the last executed command.
 *    - echo $SHELL    : Prints the path of the current shell.
 *    - jobs           : Displays the list of stopped background jobs.
 *    - fg             : Brings the most recent stopped job to the foreground
 *                       and waits for it to complete.
 *    - bg             : Resumes the most recent stopped job in the background.
 *
 * 3. Job Control
 *    - Maintains a job list for stopped processes.
 *    - Supports foreground and background job execution.
 *    - Handles SIGCHLD to manage background process completion.
 *
 * 4. Signal Handling
 *    - Custom signal handling for SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z)
 *      to prevent shell termination.
 *    - Resets signal handlers to default behavior in child processes
 *      before executing external commands.
 *    - Ensures Ctrl+C interrupts running commands without executing
 *      stale input.
 *
 * 5. Shell Behavior
 *    - Customizable shell prompt (PS1 support).
 *    - Prevents execution of incomplete or interrupted commands.
 *    - Parent process properly waits for child processes to avoid zombies.
 *
 * Design Notes:
 * -------------
 * - The shell installs its own signal handlers and manages job control.
 * - Child processes restore default signal behavior before exec().
 * - Pipes are implemented using correct file descriptor chaining.
 * - The job list is implemented using a linked list.
 *
 * This project demonstrates core operating system concepts such as
 * process creation, inter-process communication, signal handling,
 * and job control.
 *
 * Author : LAMJANE OMKAR GOURISHANKAR
 * Date   : 01/02/26
 */


#include "header.h"

char *cmmd = NULL;
char *ext_cmd[155];
char prompt[25] = "minishell$:";
int pid = 0 , status = 0;
Slist *head = NULL;
char input_string[50];

int main()
{
    system("clear");            //clear the terminal
    extract_external_commands(ext_cmd);     //extract external commands from file to 2D array
    scan_input(prompt , input_string);      //call scan_input function to scan i/p from user
}