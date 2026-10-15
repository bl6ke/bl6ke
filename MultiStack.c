 //Stack implementation using array skeleton code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// The array will store the items in the stack, first in
// index 0, then 1, etc. top will represent the index
// to the top element in the stack. If the stack is
// empty top will be -1.

#define SIZE 100
#define EMPTY '\0'

struct stack {

    char items[SIZE];

    int top;

};

void initialize(struct stack* stackPtr);
int full(struct stack* stackPtr);
int push(struct stack* stackPtr, char value);
int empty(struct stack* stackPtr);
char pop(struct stack* stackPtr);
char peek(struct stack* stackPtr);
void display(struct stack* stackPtr);

int checkBalance(char exp[]);
int priority(char ch);
int isOperator(char ch);
int isParentheses(char ch1);
char *infixToPostfix(char infix[]);

int main() {
    char exp[SIZE];

    printf("Enter Expression: ");
    if (fgets(exp, SIZE, stdin) == NULL)
        return 0;

    exp[strcspn(exp, "\n")] = '\0';

    printf("Your input expression: %s\n", exp);

    printf("\nChecking balance...\n");
    if (checkBalance(exp))
    {
        printf("VALID\n");

        char *postfix = infixToPostfix(exp);
        printf("The postfix is: %s\n", postfix);
        free(postfix);
    }

    return 0;
}

int checkBalance(char exp[]) {
    struct stack s;
    initialize(&s);

    for (int i = 0; exp[i] != '\0'; i++)
    {
        char ch = exp[i];

        if (ch == '(' || ch == '[' || ch == '{')
        {
            push(&s, ch);
        }
        else if (ch == ')' || ch == ']' || ch == '}')
        {
            if (empty(&s))
            {
                printf("INVALID for %c!!!\n", ch);
                return 0;
            }

            char open = pop(&s);
            if ((ch == ')' && open != '(') ||
                (ch == ']' && open != '[') ||
                (ch == '}' && open != '{'))
            {
                printf("INVALID for %c!!!\n", ch);
                return 0;
            }
        }
    }

    if (!empty(&s))
    {
        printf("INVALID for %c!!!\n", peek(&s));
        return 0;
    }

    return 1;
}

int priority(char ch) {
    switch (ch)
    {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '%' || ch == '^');
}

int isParentheses(char ch1) {
    return (ch1 == '(' || ch1 == ')' ||
            ch1 == '[' || ch1 == ']' ||
            ch1 == '{' || ch1 == '}');
}

// The returned string is dynamically allocated so the caller must free it
char *infixToPostfix(char infix[]) {
    struct stack s;
    initialize(&s);

    int len = strlen(infix);
    // Each character may be followed by a space, plus the null terminator
    char *postfix = malloc(2 * len + 1);
    int j = 0;

    for (int i = 0; i < len; i++)
    {
        char ch = infix[i];

        if (ch == ' ')
            continue;

        if (isalnum(ch))
        {
            while (i < len && isalnum(infix[i]))
            {
                postfix[j++] = infix[i];
                i++;
            }
            postfix[j++] = ' ';
            i--; // the for loop will move past the last character
        }
        else if (ch == '(' || ch == '[' || ch == '{')
        {
            push(&s, ch);
        }
        else if (isParentheses(ch))
        {
            while (!empty(&s) && !isParentheses(peek(&s)))
            {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            pop(&s); // discard the opening parenthesis
        }
        // '^' is right associative, so only pop strictly higher priority operators for it
        else if (isOperator(ch))
        {
            while (!empty(&s) && isOperator(peek(&s)) &&
                   (priority(peek(&s)) > priority(ch) ||
                    (priority(peek(&s)) == priority(ch) && ch != '^')))
            {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            push(&s, ch);
        }
    }

    while (!empty(&s))
    {
        postfix[j++] = pop(&s);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
    return postfix;
}

void initialize(struct stack* stackPtr) {
     stackPtr->top = -1;
}

// If the push occurs, 1 is returned. If the
// stack is full and the push can't be done, 0 is
// returned.
int push(struct stack* stackPtr, char value) {

    // Check if the stack is full.
    if (full(stackPtr))
        return 0;

    // Add value to the top of the stack and adjust the value of the top.
    stackPtr->items[stackPtr->top+1] = value;
    (stackPtr->top)++;
    return 1;
}

// Returns true iff the stack pointed to by stackPtr is full.
int full(struct stack* stackPtr) {
    return (stackPtr->top == SIZE - 1);
}

// Returns true iff the stack pointed to by stackPtr is empty.
int empty(struct stack* stackPtr) {
    return (stackPtr->top == -1);
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is popped and returned.
// Note: If the stack pointed to by stackPtr is empty, EMPTY is returned.
char pop(struct stack* stackPtr) {

    char retval;

    // Check the case that the stack is empty.
    if (empty(stackPtr))
        return EMPTY;

    // Retrieve the item from the top of the stack, adjust the top and return
    // the item.
    retval = stackPtr->items[stackPtr->top];
    (stackPtr->top)--;
    return retval;
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is returned.
// Note: If the stack pointed to by stackPtr is empty, EMPTY is returned.
char peek(struct stack* stackPtr) {

    // Take care of the empty case.
    if (empty(stackPtr))
        return EMPTY;

    // Return the desired item.
    return stackPtr->items[stackPtr->top];
}

void display(struct stack* stackPtr) {
    printf("\nPrinting the Current stack...");
    for(int i=0; i<=stackPtr->top; i++)
        printf("%c ", stackPtr->items[i]);
}
