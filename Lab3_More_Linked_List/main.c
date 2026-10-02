#include "main.h"
#include <stdio.h>
#include <stdlib.h>

node* insert_front(node *root, int item)
{
    node *temp;

    temp = (node*)malloc(sizeof(node));
    temp->data = item;
    temp->next = root;
    root = temp;

    return root;
}

node* reverse(node* head)
{
    node *main_list = head;
    node *reversed_list;
    node *temp;

    if (main_list == NULL)
        return NULL;

    reversed_list = main_list;
    main_list = main_list->next;
    reversed_list->next = NULL;

    while (main_list != NULL)
    {
        temp = main_list;
        main_list = main_list->next;
        temp->next = reversed_list;
        reversed_list = temp;
    }

    return reversed_list;
}

void insertToPlace(node* list, int val, int place)
{
    node *temp;
    int count = 1;

    temp = (node*)malloc(sizeof(node));
    temp->data = val;
    temp->next = NULL;

    while (count < place - 1 && list->next != NULL)
    {
        list = list->next;
        count++;
    }

    temp->next = list->next;
    list->next = temp;
}

void display(node* t)
{
    printf("\nPrinting your linked list.......");

    while (t != NULL)
    {
        printf("%d ", t->data);
        t = t->next;
    }

    printf("\n");
}

void freeList(node* t)
{
    node *temp;

    while (t != NULL)
    {
        temp = t;
        t = t->next;
        free(temp);
    }
}

int main()
{
    node *root = NULL;
    int ch, ele, place;

    printf("\n");

    while (1)
    {
        printf("Menu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
        if (scanf("%d", &ch) != 1)
            break;

        if (ch == 0)
        {
            printf("\nGOOD BYE>>>>\n\n");
            break;
        }

        if (ch == 1)
        {
            printf("\nEnter data (an integer): ");
            scanf("%d", &ele);
            root = insert_front(root, ele);
            display(root);
        }
        else if (ch == 2)
        {
            root = reverse(root);
            printf("List reversed.\n");
            display(root);
        }
        else if (ch == 3)
        {
            printf("\nEnter data (an integer) and place (>1) separated by space: ");
            scanf("%d %d", &ele, &place);

            if (root == NULL || place <= 1)
                printf("\nList is empty or place is not valid");
            else
                insertToPlace(root, ele, place);

            display(root);
        }
    }

    freeList(root);

    return 0;
}
