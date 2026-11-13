/* COP 3502C - Programming Assignment 5
   Monster Expedition: Sanctuary Registry
   A single unbalanced BST keyed by monster name (strcmp order). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 30

typedef struct Monster {
    char *name;
    char element[MAXSIZE + 1];
    int power;
} Monster;

typedef struct BST_Node {
    Monster *monster;
    struct BST_Node *left;
    struct BST_Node *right;
} BST_Node;

Monster *createMonster(const char *name, const char *element, int power);
void freeMonster(Monster *m);
BST_Node *createNode(Monster *m);
BST_Node *insert(BST_Node *root, BST_Node *element);
BST_Node *findNode(BST_Node *root, const char *name);
BST_Node *parent(BST_Node *root, BST_Node *node);
BST_Node *minVal(BST_Node *root);
int isLeaf(BST_Node *node);
int hasOnlyLeftChild(BST_Node *node);
int hasOnlyRightChild(BST_Node *node);
BST_Node *deleteNode(BST_Node *root, const char *name);
BST_Node *successor(BST_Node *root, const char *name);
int countRange(BST_Node *root, const char *low, const char *high);
void inorder(BST_Node *root);
void freeTree(BST_Node *root);

int main(void) {
    BST_Node *root = NULL;
    int q, i;

    if (scanf("%d", &q) != 1)
        return 0;

    for (i = 0; i < q; i++) {
        int op;
        char name[MAXSIZE + 1], other[MAXSIZE + 1];
        int power;

        if (scanf("%d", &op) != 1)
            break;

        if (op == 1) {
            scanf("%30s %30s %d", name, other, &power);
            if (findNode(root, name) != NULL) {
                printf("DUPLICATE\n");
            } else {
                root = insert(root, createNode(createMonster(name, other, power)));
                printf("INSERTED\n");
            }
        } else if (op == 2) {
            scanf("%30s", name);
            if (findNode(root, name) == NULL) {
                printf("NOT FOUND\n");
            } else {
                root = deleteNode(root, name);
                printf("DELETED\n");
            }
        } else if (op == 3) {
            BST_Node *found;
            scanf("%30s", name);
            found = findNode(root, name);
            if (found != NULL)
                printf("FOUND %s %d\n", found->monster->element, found->monster->power);
            else
                printf("NOT FOUND\n");
        } else if (op == 4) {
            BST_Node *succ;
            scanf("%30s", name);
            succ = successor(root, name);
            if (succ != NULL)
                printf("%s\n", succ->monster->name);
            else
                printf("NO SUCCESSOR\n");
        } else if (op == 5) {
            scanf("%30s %30s", name, other);
            printf("%d\n", countRange(root, name, other));
        } else if (op == 6) {
            if (root == NULL)
                printf("EMPTY\n");
            else
                inorder(root);
        }
    }

    freeTree(root);
    return 0;
}

/* Allocates a monster; the name gets exactly strlen + 1 bytes. */
Monster *createMonster(const char *name, const char *element, int power) {
    Monster *m = (Monster *)malloc(sizeof(Monster));
    m->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(m->name, name);
    strcpy(m->element, element);
    m->power = power;
    return m;
}

void freeMonster(Monster *m) {
    free(m->name);
    free(m);
}

BST_Node *createNode(Monster *m) {
    BST_Node *node = (BST_Node *)malloc(sizeof(BST_Node));
    node->monster = m;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Inserts element into the tree rooted at root; returns the root.
   The caller has already ruled out duplicates. */
BST_Node *insert(BST_Node *root, BST_Node *element) {
    if (root == NULL)
        return element;

    if (strcmp(element->monster->name, root->monster->name) > 0)
        root->right = insert(root->right, element);
    else
        root->left = insert(root->left, element);

    return root;
}

/* Returns the node holding name, or NULL if it is not in the tree. */
BST_Node *findNode(BST_Node *root, const char *name) {
    while (root != NULL) {
        int cmp = strcmp(name, root->monster->name);
        if (cmp == 0)
            return root;
        if (cmp < 0)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

/* Returns the parent of node, or NULL if node is the root. */
BST_Node *parent(BST_Node *root, BST_Node *node) {
    if (root == NULL || root == node)
        return NULL;

    while (root != NULL) {
        if (root->left == node || root->right == node)
            return root;
        if (strcmp(node->monster->name, root->monster->name) < 0)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

/* Returns the leftmost (smallest) node of a non-empty tree. */
BST_Node *minVal(BST_Node *root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

int isLeaf(BST_Node *node) {
    return node->left == NULL && node->right == NULL;
}

int hasOnlyLeftChild(BST_Node *node) {
    return node->left != NULL && node->right == NULL;
}

int hasOnlyRightChild(BST_Node *node) {
    return node->left == NULL && node->right != NULL;
}

/* Deletes the node holding name (which must be in the tree) and
   returns the root of the resulting tree. */
BST_Node *deleteNode(BST_Node *root, const char *name) {
    BST_Node *delnode, *par, *save_node, *new_del_node;
    Monster *save_monster;

    delnode = findNode(root, name);
    par = parent(root, delnode);

    /* Case 1: the node is a leaf. */
    if (isLeaf(delnode)) {
        if (par == NULL) {
            freeMonster(root->monster);
            free(root);
            return NULL;
        }

        if (par->left == delnode)
            par->left = NULL;
        else
            par->right = NULL;

        freeMonster(delnode->monster);
        free(delnode);
        return root;
    }

    /* Case 2.1: the node has only a left child. */
    if (hasOnlyLeftChild(delnode)) {
        if (par == NULL) {
            save_node = delnode->left;
            freeMonster(delnode->monster);
            free(delnode);
            return save_node;
        }

        if (par->left == delnode)
            par->left = delnode->left;
        else
            par->right = delnode->left;

        freeMonster(delnode->monster);
        free(delnode);
        return root;
    }

    /* Case 2.2: the node has only a right child. */
    if (hasOnlyRightChild(delnode)) {
        if (par == NULL) {
            save_node = delnode->right;
            freeMonster(delnode->monster);
            free(delnode);
            return save_node;
        }

        if (par->left == delnode)
            par->left = delnode->right;
        else
            par->right = delnode->right;

        freeMonster(delnode->monster);
        free(delnode);
        return root;
    }

    /* Case 3: two children. Copy the in-order successor's data, delete
       the successor's node, then store the copy in this node. */
    new_del_node = minVal(delnode->right);
    save_monster = createMonster(new_del_node->monster->name,
                                 new_del_node->monster->element,
                                 new_del_node->monster->power);

    deleteNode(root, save_monster->name);

    freeMonster(delnode->monster);
    delnode->monster = save_monster;

    return root;
}

/* Returns the node with the smallest name strictly greater than name,
   or NULL if there is none. Runs in O(h). */
BST_Node *successor(BST_Node *root, const char *name) {
    BST_Node *best = NULL;

    while (root != NULL) {
        if (strcmp(root->monster->name, name) > 0) {
            best = root;
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return best;
}

/* Counts names in [low, high], skipping subtrees that cannot qualify. */
int countRange(BST_Node *root, const char *low, const char *high) {
    if (root == NULL || strcmp(low, high) > 0)
        return 0;

    if (strcmp(root->monster->name, low) < 0)
        return countRange(root->right, low, high);

    if (strcmp(root->monster->name, high) > 0)
        return countRange(root->left, low, high);

    return 1 + countRange(root->left, low, high) + countRange(root->right, low, high);
}

void inorder(BST_Node *root) {
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%s %s %d\n", root->monster->name, root->monster->element, root->monster->power);
    inorder(root->right);
}

void freeTree(BST_Node *root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    freeMonster(root->monster);
    free(root);
}
