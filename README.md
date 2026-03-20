# MiniShell – Custom Unix Shell in C

### Author: Omkar Lamjane  
### Domain: Embedded Systems / System Programming  

---

## Project Overview
MiniShell is a simplified implementation of a Unix/Linux shell developed using the C programming language. The project demonstrates how operating systems execute commands, manage processes, and interact with users through a command-line interface.

The shell reads user input, parses commands, and executes them using system calls such as `fork()`, `execvp()`, and `wait()`.

---

## Objectives
- Understand process creation and management  
- Learn command parsing and execution  
- Implement built-in shell commands  
- Gain practical experience with system calls  

---

## Features
- Command execution (e.g., `ls`, `pwd`, `cat`)  
- Built-in commands:
  - `cd`
  - `exit`  
- Command history using a linked list  
- Dynamic memory management  
- Process handling using `fork()` and `execvp()`  
- Error handling for invalid commands  

---

## Concepts Used
- Process Management (`fork`, `exec`, `wait`)  
- Pointers and Dynamic Memory Allocation  
- Linked Lists  
- String Parsing  
- System Calls  
- File Handling (if implemented)  

---

## Project Structure

MiniShell/
│── main.c
│── shell.c
│── list.c
│── header.h
│── Makefile
│── README.md


---

## How It Works
1. The shell displays a prompt  
2. The user enters a command  
3. The command is parsed into tokens  
4. The shell checks:
   - If it is a built-in command, it executes internally  
   - Otherwise, it creates a child process  
5. The command is executed using `execvp()`  
6. The parent process waits for completion  
7. The loop continues  

---

## Compilation and Execution
```bash
gcc *.c -o minishell
./minishell
Sample Output
minishell$ ls
file1.c file2.c

minishell$ pwd
/home/user/project

minishell$ exit
Challenges Faced

Managing memory safely for command history

Handling string parsing correctly

Avoiding pointer-related issues

Differentiating built-in and external commands

Future Enhancements

Support for pipes (|)

Input/output redirection (>, <)

Background process handling (&)

Environment variable support

Command auto-completion

Key Learning Outcome

This project provided a strong foundation in system-level programming and improved understanding of how shells interact with the operating system.
