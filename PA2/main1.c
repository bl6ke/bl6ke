/*
 * COP 3502C - PA2: Recursion / Monster Lineup
 * main1.c - Base-case constraint checking.
 *
 * Generates every permutation of monster indices with the recursive
 * used[] technique and checks all constraints only once a permutation
 * is complete (at the base case). The first permutation that satisfies
 * every constraint is printed and the search stops.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 12
#define MAXC 1000
#define MAXLEN 30
#define MAXTYPE 32

#define BEFORE 0
#define IMMEDIATELY_BEFORE 1
#define POSITION 2
#define FIRST_ELEMENT 3
#define LAST_ELEMENT 4
#define NO_ADJACENT_ELEMENT 5
#define ELEMENT_BEFORE_ALL 6

typedef struct {
    char *name;                  /* dynamically allocated to fit the name */
    char element[MAXLEN + 1];    /* statically allocated */
} Monster;

typedef struct {
    int type;
    int a;                       /* monster index */
    int b;                       /* second monster index, or 1-based position */
    char elemA[MAXLEN + 1];
    char elemB[MAXLEN + 1];
} Constraint;

/* The only two global arrays. */
Monster monsters[MAXN];
Constraint constraints[MAXC];

/* Returns the slot of target in perm[idx..len-1], or -1 if it is not there. */
int recursiveFindPosition(int perm[], int len, int target, int idx) {
    if (idx >= len)
        return -1;
    if (perm[idx] == target)
        return idx;
    return recursiveFindPosition(perm, len, target, idx + 1);
}

/* Returns 1 if two neighbors in perm[idx-1..len-1] both have element elem. */
int recursiveHasAdjacent(int perm[], int len, char elem[], int idx) {
    if (idx >= len)
        return 0;
    if (strcmp(monsters[perm[idx - 1]].element, elem) == 0 &&
        strcmp(monsters[perm[idx]].element, elem) == 0)
        return 1;
    return recursiveHasAdjacent(perm, len, elem, idx + 1);
}

/*
 * Returns 1 if no monster with elemA appears after a monster with elemB,
 * scanning perm[idx..len-1]. seenB records whether an elemB monster has
 * already been seen earlier in the lineup.
 */
int recursiveElementOrder(int perm[], int len, char elemA[], char elemB[],
                          int idx, int seenB) {
    if (idx >= len)
        return 1;
    if (seenB && strcmp(monsters[perm[idx]].element, elemA) == 0)
        return 0;
    if (strcmp(monsters[perm[idx]].element, elemB) == 0)
        seenB = 1;
    return recursiveElementOrder(perm, len, elemA, elemB, idx + 1, seenB);
}

/* Returns 1 if the complete lineup perm satisfies constraint con. */
int checkConstraint(int perm[], int n, Constraint *con) {
    int posA, posB;

    switch (con->type) {
    case BEFORE:
        posA = recursiveFindPosition(perm, n, con->a, 0);
        posB = recursiveFindPosition(perm, n, con->b, 0);
        return posA < posB;
    case IMMEDIATELY_BEFORE:
        posA = recursiveFindPosition(perm, n, con->a, 0);
        posB = recursiveFindPosition(perm, n, con->b, 0);
        return posA + 1 == posB;
    case POSITION:
        return con->b >= 1 && con->b <= n && perm[con->b - 1] == con->a;
    case FIRST_ELEMENT:
        return strcmp(monsters[perm[0]].element, con->elemA) == 0;
    case LAST_ELEMENT:
        return strcmp(monsters[perm[n - 1]].element, con->elemA) == 0;
    case NO_ADJACENT_ELEMENT:
        return !recursiveHasAdjacent(perm, n, con->elemA, 1);
    case ELEMENT_BEFORE_ALL:
        return recursiveElementOrder(perm, n, con->elemA, con->elemB, 0, 0);
    }
    return 0;
}

/* Returns 1 if the complete lineup satisfies constraints idx..numC-1. */
int recursiveCheckConstraints(int perm[], int n, int numC, int idx) {
    if (idx >= numC)
        return 1;
    if (!checkConstraint(perm, n, &constraints[idx]))
        return 0;
    return recursiveCheckConstraints(perm, n, numC, idx + 1);
}

/* Prints the monsters in perm[idx..n-1], one per line. */
void recursivePrintLineup(int perm[], int n, int idx) {
    if (idx >= n)
        return;
    printf("%s %s\n", monsters[perm[idx]].name, monsters[perm[idx]].element);
    recursivePrintLineup(perm, n, idx + 1);
}

/* Frees the names of monsters[0..idx]. */
void recursiveFreeMonsters(int idx) {
    if (idx < 0)
        return;
    free(monsters[idx].name);
    monsters[idx].name = NULL;
    recursiveFreeMonsters(idx - 1);
}

/*
 * Fills perm[k..n-1] with every unused monster index in turn. Constraints
 * are checked only when the permutation is complete. Returns 1 once a valid
 * lineup has been printed so the remaining search is skipped.
 */
int permute(int perm[], int used[], int k, int n, int numC) {
    int found;

    if (k == n) {
        if (recursiveCheckConstraints(perm, n, numC, 0)) {
            recursivePrintLineup(perm, n, 0);
            return 1;
        }
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = 1;
            perm[k] = i;
            found = permute(perm, used, k + 1, n, numC);
            used[i] = 0;
            if (found)
                return 1;
        }
    }
    return 0;
}

/* Reads one constraint from input into con. Returns 1 on success. */
int readConstraint(Constraint *con) {
    char type[MAXTYPE];

    if (scanf("%31s", type) != 1)
        return 0;

    con->a = con->b = -1;
    con->elemA[0] = con->elemB[0] = '\0';

    if (strcmp(type, "BEFORE") == 0) {
        con->type = BEFORE;
        return scanf("%d %d", &con->a, &con->b) == 2;
    }
    if (strcmp(type, "IMMEDIATELY_BEFORE") == 0) {
        con->type = IMMEDIATELY_BEFORE;
        return scanf("%d %d", &con->a, &con->b) == 2;
    }
    if (strcmp(type, "POSITION") == 0) {
        con->type = POSITION;
        return scanf("%d %d", &con->a, &con->b) == 2;
    }
    if (strcmp(type, "FIRST_ELEMENT") == 0) {
        con->type = FIRST_ELEMENT;
        return scanf("%30s", con->elemA) == 1;
    }
    if (strcmp(type, "LAST_ELEMENT") == 0) {
        con->type = LAST_ELEMENT;
        return scanf("%30s", con->elemA) == 1;
    }
    if (strcmp(type, "NO_ADJACENT_ELEMENT") == 0) {
        con->type = NO_ADJACENT_ELEMENT;
        return scanf("%30s", con->elemA) == 1;
    }
    if (strcmp(type, "ELEMENT_BEFORE_ALL") == 0) {
        con->type = ELEMENT_BEFORE_ALL;
        return scanf("%30s %30s", con->elemA, con->elemB) == 2;
    }
    return 0;
}

int main(void) {
    int n, numC;
    int perm[MAXN];
    int used[MAXN] = {0};
    char nameBuf[MAXLEN + 1];

    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN)
        return 1;

    for (int i = 0; i < n; i++) {
        if (scanf("%30s %30s", nameBuf, monsters[i].element) != 2) {
            recursiveFreeMonsters(i - 1);
            return 1;
        }
        monsters[i].name = malloc(strlen(nameBuf) + 1);
        if (monsters[i].name == NULL) {
            recursiveFreeMonsters(i - 1);
            return 1;
        }
        strcpy(monsters[i].name, nameBuf);
    }

    if (scanf("%d", &numC) != 1 || numC < 0 || numC > MAXC) {
        recursiveFreeMonsters(n - 1);
        return 1;
    }

    for (int i = 0; i < numC; i++) {
        if (!readConstraint(&constraints[i])) {
            recursiveFreeMonsters(n - 1);
            return 1;
        }
    }

    permute(perm, used, 0, n, numC);

    recursiveFreeMonsters(n - 1);
    return 0;
}
