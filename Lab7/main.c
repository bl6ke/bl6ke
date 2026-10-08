/*
 * CS1 Lab 7 - Recursion with Linked List
 *
 * Sorted linked list insertion and deletion, plus print, free and copy,
 * all implemented recursively.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------- Memory leak detector ----------------
 * Every malloc/free in this file goes through xmalloc/xfree, which keep a
 * list of the blocks still allocated. At exit, report_mem_leak() writes any
 * block that was never freed to leak_info.txt.
 */

#define LEAK_FILE_NAME_LENGTH 256
#define LEAK_OUTPUT_FILE      "leak_info.txt"

typedef struct mem_leak {
    void *address;
    unsigned int size;
    char file_name[LEAK_FILE_NAME_LENGTH];
    unsigned int line;
    struct mem_leak *next;
} MEM_LEAK;

static MEM_LEAK *leak_start = NULL;

void *xmalloc(unsigned int size, const char *file, unsigned int line)
{
    void *ptr = malloc(size);
    if (ptr != NULL)
    {
        MEM_LEAK *info = (MEM_LEAK*)malloc(sizeof(MEM_LEAK));
        info->address = ptr;
        info->size = size;
        strncpy(info->file_name, file, LEAK_FILE_NAME_LENGTH - 1);
        info->file_name[LEAK_FILE_NAME_LENGTH - 1] = '\0';
        info->line = line;
        info->next = leak_start;
        leak_start = info;
    }
    return ptr;
}

void xfree(void *mem_ref)
{
    MEM_LEAK **cur = &leak_start;
    while (*cur != NULL)
    {
        if ((*cur)->address == mem_ref)
        {
            MEM_LEAK *temp = *cur;
            *cur = temp->next;
            free(temp);
            break;
        }
        cur = &(*cur)->next;
    }
    free(mem_ref);
}

void report_mem_leak(void)
{
    FILE *fp = fopen(LEAK_OUTPUT_FILE, "w");
    MEM_LEAK *info;

    if (fp != NULL)
    {
        fprintf(fp, "Memory Leak Summary\n");
        fprintf(fp, "-----------------------------------\n");
        for (info = leak_start; info != NULL; info = info->next)
        {
            fprintf(fp, "address : %p\n", info->address);
            fprintf(fp, "size    : %u bytes\n", info->size);
            fprintf(fp, "file    : %s\n", info->file_name);
            fprintf(fp, "line    : %u\n", info->line);
            fprintf(fp, "-----------------------------------\n");
        }
        fclose(fp);
    }

    while (leak_start != NULL)
    {
        MEM_LEAK *temp = leak_start;
        leak_start = leak_start->next;
        free(temp);
    }
}

#define malloc(size)  xmalloc(size, __FILE__, __LINE__)
#define free(mem_ref) xfree(mem_ref)

/* ------------------------------------------------------ */

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
