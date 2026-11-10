/* COP 3502C PA4
This program is written by: Your Full Name */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXSIZE 30
#define NUMSCORES 4
#define POWER 0
#define SPEED 1
#define STAMINA 2
#define TOTAL 3
#define BASECASESIZE 30

typedef struct Monster {
    char *name;
    char element[MAXSIZE + 1];
    int scores[NUMSCORES];
} Monster;

// Function prototypes
Monster *readMonster(void);
int compareTo(Monster *m1, Monster *m2, int key);
void swap(Monster **a, Monster **b);
void insertionSort(Monster **list, int low, int high, int key);
void mergeSort(Monster **list, int n, int key);
void mergeSortRec(Monster **list, int low, int high, int key);
void merge(Monster **list, int low, int mid, int high, int key);
void quickSort(Monster **list, int n, int key);
void quickSortRec(Monster **list, int low, int high, int key);
int partition(Monster **list, int low, int high, int key);
void printRanking(Monster **list, int n, int key);
void freeMonsters(Monster **list, int n);

int main(void) {
    int n, i;

    srand(time(NULL));

    scanf("%d", &n);

    // Allocate the two pointer arrays that will share the same Monsters.
    Monster **overallList = malloc(n * sizeof(Monster *));
    Monster **speedList = malloc(n * sizeof(Monster *));

    // Each Monster is allocated exactly once; both lists point to it.
    for (i = 0; i < n; i++) {
        overallList[i] = readMonster();
        speedList[i] = overallList[i];
    }

    // Overall ranking: Merge Sort by TOTAL.
    mergeSort(overallList, n, TOTAL);

    // Speed ranking: Quick Sort by SPEED.
    quickSort(speedList, n, SPEED);

    printf("Overall Expedition Ranking\n");
    printRanking(overallList, n, TOTAL);

    printf("\nSpeed Challenge Ranking\n");
    printRanking(speedList, n, SPEED);

    // Free each Monster once (through one list only), then both arrays.
    freeMonsters(overallList, n);
    free(overallList);
    free(speedList);

    return 0;
}

// Reads one monster from input, allocates it, and computes its TOTAL score.
Monster *readMonster(void) {
    char buffer[MAXSIZE + 1];
    Monster *m = malloc(sizeof(Monster));

    scanf("%s", buffer);

    // Allocate exactly enough memory for the name.
    m->name = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(m->name, buffer);

    scanf("%s", m->element);
    scanf("%d %d %d", &m->scores[POWER], &m->scores[SPEED], &m->scores[STAMINA]);

    m->scores[TOTAL] = m->scores[POWER] + m->scores[SPEED] + m->scores[STAMINA];

    return m;
}

// Returns negative if m1 comes before m2 (larger score first), positive if
// m2 comes before m1. Ties on the score are broken alphabetically by name.
int compareTo(Monster *m1, Monster *m2, int key) {
    if (m1->scores[key] != m2->scores[key])
        return m2->scores[key] - m1->scores[key];

    return strcmp(m1->name, m2->name);
}

// Swaps two Monster pointers.
void swap(Monster **a, Monster **b) {
    Monster *temp = *a;
    *a = *b;
    *b = temp;
}

// Sorts only list[low..high] according to key.
void insertionSort(Monster **list, int low, int high, int key) {
    int i, j;
    Monster *hand;

    for (i = low + 1; i <= high; i++) {
        hand = list[i];

        // Shift elements that should come after hand one spot to the right.
        for (j = i - 1; j >= low; j--) {
            if (compareTo(list[j], hand, key) > 0)
                list[j + 1] = list[j];
            else
                break;
        }

        list[j + 1] = hand;
    }
}

// Merge Sort wrapper for an array of size n.
void mergeSort(Monster **list, int n, int key) {
    mergeSortRec(list, 0, n - 1, key);
}

// Recursive hybrid Merge Sort on list[low..high].
void mergeSortRec(Monster **list, int low, int high, int key) {
    // Small subarray: use Insertion Sort instead of dividing further.
    if (high - low + 1 <= BASECASESIZE) {
        insertionSort(list, low, high, key);
        return;
    }

    int mid = (low + high) / 2;

    mergeSortRec(list, low, mid, key);
    mergeSortRec(list, mid + 1, high, key);
    merge(list, low, mid, high, key);
}

// Merges the sorted halves list[low..mid] and list[mid+1..high].
void merge(Monster **list, int low, int mid, int high, int key) {
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // Temporary arrays for the left and right halves.
    Monster **L = malloc(n1 * sizeof(Monster *));
    Monster **R = malloc(n2 * sizeof(Monster *));

    for (i = 0; i < n1; i++)
        L[i] = list[low + i];
    for (j = 0; j < n2; j++)
        R[j] = list[mid + 1 + j];

    i = 0;
    j = 0;
    k = low;

    // Copy the element that comes first back into list.
    while (i < n1 && j < n2) {
        if (compareTo(L[i], R[j], key) <= 0) {
            list[k] = L[i];
            i++;
        }
        else {
            list[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements of L.
    while (i < n1) {
        list[k] = L[i];
        i++;
        k++;
    }

    // Copy any remaining elements of R.
    while (j < n2) {
        list[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Quick Sort wrapper for an array of size n.
void quickSort(Monster **list, int n, int key) {
    quickSortRec(list, 0, n - 1, key);
}

// Recursive hybrid Quick Sort on list[low..high].
void quickSortRec(Monster **list, int low, int high, int key) {
    // Small subarray: use Insertion Sort instead of partitioning further.
    if (high - low + 1 <= BASECASESIZE) {
        insertionSort(list, low, high, key);
        return;
    }

    int split = partition(list, low, high, key);

    quickSortRec(list, low, split - 1, key);
    quickSortRec(list, split + 1, high, key);
}

// Partitions list[low..high] around a randomly chosen pivot and returns
// the pivot's final index.
int partition(Monster **list, int low, int high, int key) {
    // Pick a random pivot and move it to the front.
    int i = low + rand() % (high - low + 1);
    swap(&list[low], &list[i]);

    int lowpos = low;
    low++;

    while (low <= high) {
        // Move low forward while elements belong before the pivot.
        while (low <= high && compareTo(list[low], list[lowpos], key) <= 0)
            low++;

        // Move high backward while elements belong after the pivot.
        while (high >= low && compareTo(list[high], list[lowpos], key) > 0)
            high--;

        // Swap the out-of-place pair.
        if (low < high)
            swap(&list[low], &list[high]);
    }

    // Put the pivot into its final position.
    swap(&list[lowpos], &list[high]);

    return high;
}

// Prints a ranking using the score selected by key.
void printRanking(Monster **list, int n, int key) {
    int i;
    for (i = 0; i < n; i++)
        printf("%d. %-15s %d\n", i + 1, list[i]->name, list[i]->scores[key]);
}

// Frees every Monster and its name exactly once.
void freeMonsters(Monster **list, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(list[i]->name);
        free(list[i]);
    }
}
