#include <stdio.h>
#include <string.h>
#include "header.h"


// for inserting pid and command into the linked list
void insert_at_first(Slist **head)
{
    Slist *new = malloc(sizeof(Slist));        // allocate memory for new list node
    if(new == NULL)
    {
        printf("Memory not allocated\n");      // handle allocation failure
        return;
    }

    new->pid = pid;                            // store process id
    new->cmd = malloc(strlen(input_string) + 1); // allocate memory for command string
    if (new->cmd == NULL)
    {
        free(new);                             // free node if command allocation fails
        printf("Memory not allocated for cmd\n");
        return;
    }

    strcpy(new->cmd, input_string);            // copy command into node
    new->link = NULL;                          // initialize link pointer

    if((*head) == NULL)                        // if list is empty
    {  
        *(head) = new;                         // make new node the head
    }
    else
    {
        new->link = (*head);                   // link new node to current head
        (*head) = new;                         // update head to new node
    }
}

// for printing the linked list
void print_list(Slist *head)
{
    for(int i = 0; head != NULL; i++)           // traverse the linked list
    {
        if(i == 0)
        {
            printf("[%d]+   Stopped             %s\n", i+1, head->cmd); // mark current job
        }
        else if(i == 1)
        {
            printf("[%d]-   Stopped             %s\n", i+1, head->cmd); // mark previous job
        }
        else
        {
            printf("[%d]    Stopped             %s\n", i+1, head->cmd); // print other jobs
        }
        head = head->link;                     // move to next node
    }
}

// for deleting the first node in the list
void delete_first(Slist **head)
{
    if((*head) == NULL)                        // check if list is empty
    {
        printf("-bash: fg: current: no such job\n"); // no job to delete
    }
    else
    {
        Slist *temp = *head;                   // store current head
        (*head) = (*head)->link;               // move head to next node

        free(temp->cmd);                       // free command string
        free(temp);                            // free node memory
    }
}
