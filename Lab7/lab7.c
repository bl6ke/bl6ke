/*
 * CS1 Lab 7 - Recursion with Linked List
 *
 * Sorted linked list insertion and deletion, plus print, free and copy,
 * all implemented recursively.
 */

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

struct node {
    int data;
    struct node *next;
};

struct node* insert(struct node* list, int d);
struct node* del(struct node* list, int d);
void print(struct node *list);
void freeList(struct node* list);
void copy(struct node *q, struct node **s);

// Inserts d into the sorted list and returns the (possibly new) head.
struct node* insert(struct node* list, int d)
{
    // Empty list or d belongs before the current node: create a new node here.
    if (list == NULL || d < list->data)
    {
        struct node *temp = (struct node*)malloc(sizeof(struct node));
        temp->data = d;
        temp->next = list;
        return temp;
    }

    // Otherwise d goes somewhere after this node.
    list->next = insert(list->next, d);
    return list;
}

// Deletes the first occurrence of d from the list and returns the new head.
struct node* del(struct node* list, int d)
{
    // Reached the end without finding d.
    if (list == NULL)
        return NULL;

    // Found it: unlink this node and free it.
    if (list->data == d)
    {
        struct node *rest = list->next;
        free(list);
        return rest;
    }

    list->next = del(list->next, d);
    return list;
}

// Prints the list in the format ->a->b->c
void print(struct node *list)
{
    if (list == NULL)
        return;

    printf("->%d", list->data);
    print(list->next);
}

// Frees every node of the list.
void freeList(struct node* list)
{
    if (list == NULL)
        return;

    freeList(list->next);
    free(list);
}

// Makes a copy of list q and stores its head in *s.
void copy(struct node *q, struct node **s)
{
    if (q == NULL)
    {
        *s = NULL;
        return;
    }

    *s = (struct node*)malloc(sizeof(struct node));
    (*s)->data = q->data;
    copy(q->next, &((*s)->next));
}

int main( ) {

    atexit(report_mem_leak); // memory leak detector

    int number = 0, choice=0;
    struct node *pList=NULL;
    struct node *nList = NULL;

    // Let the user add values until they enter -1.
    while(choice!= 4)
    {
        // Get the operation.
        printf("\nDo you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
        scanf("%d", &choice);

        printf("Your choice is %d\n", choice);

        // Execute the operation.
        if (choice == 1)
        {
            // Get the number.
            printf("Enter the value to insert\n");
            scanf("%d", &number);
            pList = insert(pList, number);
            // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            //printf("\n");
        }
        else if (choice == 2)
        {   // Get the number.
            printf("Enter the value to delete.\n");
            scanf("%d", &number);
            pList = del(pList, number);
            // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            //printf("\n");
        }
        else if (choice == 3)
        {
            if (nList)
                freeList(nList);

            copy(pList, &nList); //passing reference of nList as it is not returning anything
            // Look at the list.
            printf("Items in NEW linked list: ");
            print(nList);
            // printf("\n");
        }
        else
        {
            break;
        }
    }
    freeList(nList);
    freeList(pList);
    printf("\nBye..\n");
    return 0;
}
