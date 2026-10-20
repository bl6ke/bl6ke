// X-Stack1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20
#define EMPTY -1
//struct for stack
typedef struct stack {
  int items[SIZE];
  int top;
} stack;

//node struct for a linked list (it has no relation with stack)
typedef struct node {
    int data;
    struct node *next;
} node;

void initialize(stack *stackPtr);
int full(stack *stackPtr);
int push(stack *stackPtr, int value);
int empty(stack *stackPtr);
int pop(stack *stackPtr);
int peek(stack *stackPtr);
void displayStack(stack *stackPtr);
void displayList(node *head);
int checkMirror(char str[]); // you need to write this function
node* transferStackToList(stack *s); //you need to write this function

//note that you don't need to modify any other places of the code. Just need to write the two mentioned functions and then test them

int main() {

  char str1[20] = "madam";

  // Please write the checkMirror function appropriately so that the following  function calls work

  printf("%s mirror status: %d\n", str1, checkMirror(str1)); // this should print 1
  char str2[20] = "tata";
  printf("%s mirror status: %d\n", str2, checkMirror(str2)); // this should print 0
  char str3[20] = "dad";
  printf("%s mirror status: %d\n", str3, checkMirror(str3)); // this should print 1
  char str4[20] = "taat";
  printf("%s mirror status: %d\n", str4, checkMirror(str4)); // this should print 1
  char str5[20] = "maddam";
  printf("%s mirror status: %d\n", str5, checkMirror(str5)); // this should print 1
  char str6[20] = "madeam";
  printf("%s mirror status: %d\n", str6, checkMirror(str6)); // this should print 0

  //the following piece of code push all the items of an array to a stack
  int arr[5] = {5, 10, 15, 20, 25};
  stack st;
  initialize(&st);
  for(int i=0; i<5; i++)
    push(&st, arr[i]);
  displayStack(&st); //it displays the current status of the stack

 //write the tranferStackToList function
 node* list = transferStackToList(&st); //you have to write this function 
 displayList(list); //it should print 25, 20, 15, 10, 5 (in this order)
 displayStack(&st); //it should print empty

 //no need to free any memory for the linked list as part of the exam
  return 0;
}

// initializes top to -1, setting an empty stack
void initialize(stack *stackPtr) { stackPtr->top = -1; }

// push function for a stack
int push(stack *stackPtr, int value) {

  // Check if the stack is full.
  if (full(stackPtr))
    return 0;

  // Add value to the top of the stack and adjust the value of the top.
  stackPtr->items[stackPtr->top + 1] = value;
  (stackPtr->top)++;
  return 1;
}

// Returns true iff the stack pointed to by stackPtr is full.
int full(stack *stackPtr) { return (stackPtr->top == SIZE - 1); }

// check if a stack is empty
int empty(stack *stackPtr) { return (stackPtr->top == -1); }

// implements regular pop function of a stakc
int pop(stack *stackPtr) {
  int retval;
  // Check the case that the stack is empty.
  if (empty(stackPtr))
    return EMPTY;
  retval = stackPtr->items[stackPtr->top];
  (stackPtr->top)--;
  return retval;
}

// implements regular peek function of a stack
int peek(stack *stackPtr) {

  if (empty(stackPtr))
    return EMPTY;

  // Return the desired item.
  return stackPtr->items[stackPtr->top];
}

void displayStack(stack *stackPtr) {
  if(empty(stackPtr))
  {
    printf("\nStack is empty\n");
    return;
  }
  printf("\nPrinting the Current stack (first to last): ");
  for (int i = 0; i <= stackPtr->top; i++)
    printf("%d ", stackPtr->items[i]);
}

void displayList(node *head)
{
  if(head == NULL)
    printf("\nLinked list is empty!\n");
  else
  {
    printf("\nLinked list contents: ");
    while(head)
    {
      printf("%d ", head->data);
      head = head->next;

    }
  }
}


/*******************Your main question starts here **************/
/****You have to write the following two functions so that the main function prints the correct output *******/
/***************************************************************/

// Function to check if a string is a mirror. Read the restrictions below
int checkMirror(char str[]) {
  /* A string is a mirror if its reverse is the same as the original string. For example, madam, dad, taat, maddam, etc are mirrir strings, but tata, madeam are not mirror string.
  This function should return 1 if it the str is mirrir, otherwise it should return 0. 
  You must use a stack and stack operations during this process. Stack related functions are already provided. You just need to declare a stack, initialize it,  and use that appropriately. You must not reverse the string or write any other helper function in your logic*/

  stack s;
  initialize(&s);
  int len = strlen(str);

  // push every character onto the stack
  for (int i = 0; i < len; i++)
    push(&s, str[i]);

  // popping gives the characters in reverse order; compare with the original
  for (int i = 0; i < len; i++)
    if (pop(&s) != str[i])
      return 0;

  return 1;
}


// Function to transfer stack elements to a linked list. You need to write this function. Read the restrictions below
node* transferStackToList(stack *s) {

  /*this function should pop all the items from the stack and insert them to a linked list in the same order they are popped from the stack and finally return the head of the new linked list. For example if the stack contains 5 10 15 20 25 (where 25 is the top), then the linked list should contain 25 20 15 10 5
  Restrictions: You are not allowed to write any helper function for processing this*/

  node *head = NULL;
  node *tail = NULL;

  while (!empty(s)) {
    node *temp = (node *)malloc(sizeof(node));
    temp->data = pop(s);
    temp->next = NULL;

    // insert at the end so the list keeps the popped order
    if (head == NULL)
      head = temp;
    else
      tail->next = temp;
    tail = temp;
  }

  return head;
}
