//This code shows performing various operations on linked list recursively.
// insert, delete, free, display, copy
#include<stdio.h>
#include <stdlib.h>
#include	"leak_detector_c.h"

struct node {
     int data;
     struct node  *next;
};

/******functions you need to complete RECURSIVELY******/

/* together in the lab */
struct node* insert(struct node* list,int d );
struct node* del(struct node* list,int d );
void copy ( struct node *q, struct node **s );

/* by yourself */
void print(struct node *list);
void freeList(struct node* list);


int main( ) {

    atexit(report_mem_leak); //for leak detector
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
        {    // Get the number.
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


/* copies a linked list into another */
void copy ( struct node *source, struct node **dest )
{
    if ( source != NULL )
    {
      //allocate memory for new copy  
      *dest = (struct node*)malloc(sizeof(struct node));

      //copy the data (double dereference is needed)
      (*dest)->data = source->data;

      //set next to null
      (*dest)->next = NULL;

      //call recursively with source next and reference of nList-> next
      copy(source->next, &((*dest)->next));
    }
    else
      *dest = NULL;
}

// Recursively inserts item into the LL pointed to by list.
struct node* insert(struct node *list,int item) {

    // Node is at front, insert and return.
    if (list == NULL || item <= list->data) {
        //create new struct
        struct node *pNew = (struct node*)malloc(sizeof(struct node));

        //fill the data
        pNew->data = item;

        //connect the node to the list
        pNew->next = list;

        //return new node which is now the head of the list
        return pNew;
    }

    // Recursively insert and return the front of the list.
    list->next = insert(list->next, item);

    //after all thr recursive calls end, return list
    return list;
}

// Recursively deletes the first node storing item in the list pointed to
// by list. If no such node exists, no changes are made. a pointer to the front
// of the resulting list is returned.
struct node* del(struct node *list, int item) {

    // Simple case: list is empty
    if (list == NULL)
        //return something
        return NULL;

    //if we find this node with the item:
    // Free this node and return a pointer to the rest of the list.
    if (list->data == item) {
        //store the next node
        struct node *rest = list->next;

        //free this node
        free(list);

        //returned new stored head
        return rest;
    }

    // Recursively delete and return the front of the list.
    list->next = del(list->next, item);
    return list;
}


// COMPLETE THE FOLLOWING TWO FUNCTIONS BY YOURSELF, RECUSRSIVELY!

//prints the contents of a linked list in the following form:
// example:  ->2->3->4->6->9
void print(struct node *list) {

    // Iterate through each item and print it!
    //stop when we reach the end of the list (when list is null)
    if(list != NULL) {
        //print this data
        printf("->%d", list->data);

        //call recursively
        print(list->next);
    }

  //no need to retunr anyhting, this is a void function
}

// Frees each node in the LL pointed to by list.
void freeList(struct node* list) {

    // We can stop, when we reach the end of the list
    if (list == NULL) 
        return;

    // Free the rest of the list.
    if (list->next != NULL)
        freeList(list->next);

    // Free first node.
    free(list);
}
