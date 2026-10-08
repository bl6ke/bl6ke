// CS1 Lab 7 - Recursion with Linked List

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

// Inserts d in sorted order
struct node* insert(struct node* list, int d)
{
    if (list == NULL || d < list->data)
    {
        struct node *temp = (struct node*)malloc(sizeof(struct node));
        temp->data = d;
        temp->next = list;
        return temp;
    }

    list->next = insert(list->next, d);
    return list;
}

// Deletes the first node containing d
struct node* del(struct node* list, int d)
{
    if (list == NULL)
        return NULL;

    if (list->data == d)
    {
        struct node *rest = list->next;
        free(list);
        return rest;
    }

    list->next = del(list->next, d);
    return list;
}

void print(struct node *list)
{
    if (list == NULL)
        return;

    printf("->%d", list->data);
    print(list->next);
}

void freeList(struct node* list)
{
    if (list == NULL)
        return;

    freeList(list->next);
    free(list);
}

// Copies list q into *s
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

    atexit(report_mem_leak);

    int number = 0, choice=0;
    struct node *pList=NULL;
    struct node *nList = NULL;

    while(choice!= 4)
    {
        printf("\nDo you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
        scanf("%d", &choice);

        printf("Your choice is %d\n", choice);

        if (choice == 1)
        {
            printf("Enter the value to insert\n");
            scanf("%d", &number);
            pList = insert(pList, number);
            printf("Items in linked list: ");
            print(pList);
        }
        else if (choice == 2)
        {
            printf("Enter the value to delete.\n");
            scanf("%d", &number);
            pList = del(pList, number);
            printf("Items in linked list: ");
            print(pList);
        }
        else if (choice == 3)
        {
            if (nList)
                freeList(nList);

            copy(pList, &nList);
            printf("Items in NEW linked list: ");
            print(nList);
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
