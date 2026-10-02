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
