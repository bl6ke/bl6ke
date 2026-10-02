/* Your Name
This code implements some basic operations of singly linked list like inserting in the beginning and end, delete operation, and display operation
*/

#include "main.h"
#include<stdio.h>
#include<stdlib.h>

//this function takes an item and insert it in the linked list pointed by root.
node* insert_front(node *root, int item)
{
	node *temp;
	//create a new node and fill-up the node
	temp= (node *) malloc(sizeof(node));
	temp->data=item;
	temp->next=NULL;
	if(root==NULL) //if there is no node in the linked list
		root=temp;
	else //there is an existing linked list, so put existing root after temp
	{
		temp->next = root; //put the existing root after temp
		root = temp; //make the temp as the root!
	}
	return root;

}

void display(node* t)
{
  	printf("\nPrinting your linked list.......");

	while(t!=NULL)
	{
		printf("%d ",t->data);
		t=t->next;
	}

}

/*
	Write a function that takes the head of a linked list and then
	reverse the list. Finally, it returns the new head of the linked list. For example, if you pass
	the following linked list to the function: 10->20->15->17->NULL, the linked list will be
	converted into this: 17->15->20->10->NULL.
*/
node* reverse(node* head)
{
	// no need to reverse if head is null
	// or there is only 1 node.
	if(head==NULL || head->next==NULL)
		return head;

	// a. Lets say the list passed to the function is called main_list. Initialize this list. Think carefully about the assignment.
	node *main_list = head->next;

	// b1. Use another node pointer called reversed_list and assign it to point to the head parameter
	// b2. Assign next to NULL since head is now the tail of the reversed-List 
	node *reversed_list = head;
	reversed_list->next = NULL;

	// c1. Traverse the main_list 
	// c2. For each node in the main_list, make it point to reverse_list's head using a  
	node *temp;
	while(main_list!=NULL)
	{
		temp = main_list;
		main_list = main_list->next;
		temp->next = reversed_list;
		reversed_list = temp;
	}

	// Return the new list
	return reversed_list;
}

/* 	Write a function that takes in a pointer to
	the head of a linked list, a value to insert into the list, val, and a location in the list in which to
	insert it, place, which is guaranteed to be greater than 1, and does the insertion. If place
	number of items aren’t in the list, just insert the item in the back of the list. You can assume
	that the linked list into which the inserted item is being added is not empty. 
*/
void insertToPlace(node* list, int val, int place) {
	// Handle case where list is empty or place is not valid (i.e., place is not greater than 1 according to description)
	if(list==NULL || place<=1)
	{
		printf("\nList is empty or place is not valid");
		return;
	}

	// a. Before insertation create a temp (newNode) and fill-up the data with the val parameter.
	node *newNode = (node *) malloc(sizeof(node));
	newNode->data = val;
	newNode->next = NULL;

	// b. User a counter variable and traverse the list until you reach to the end or you reach up to the place
	// Iterate to the spot BEFORE place, 
	// the NULL check ensures we dont go off the list if place is too high.
	int count = 1;
	while(count < place-1 && list->next!=NULL)
	{
		list = list->next;
		count++;
	}

	// c. As soon as you stop traversing, add the temp (newNode) node
	newNode->next = list->next;
	list->next = newNode;

	// d. Implementation done. In the main function, display the list after reversing it by calling the display function.
}


int main()
{
	node *root=NULL; //very important line. Otherwise all function will fail
	node *t;
	int ch,ele,v, del;
	while(1)
	{
		printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
	    scanf("%d",&ch);
		if(ch==0)
		{
			printf("\nGOOD BYE>>>>\n");
			break;
		}
		if(ch==1)
		{
			printf("\nEnter data (an integer): ");
			scanf("%d",&ele);
			root = insert_front(root, ele);

            display(root);

		}
		if(ch==2)
		{
			root = reverse(root);
			printf("List reversed.\n");
			display(root);

		}
		if(ch==3)
		{
		    int place;
			printf("\nEnter data (an integer) and place (>1) separated by space: ");
			scanf("%d %d",&ele, &place);
			insertToPlace(root, ele, place);
			
			// d. In the main function, display the list after reversing it by calling the display function.
            display(root);

		}

	}
  return 0;
}
