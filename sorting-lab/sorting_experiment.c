/* COP3502C Computer Science 1
 * Sorting Algorithm Runtime Experiment
 *
 * Compares execution times of selection, bubble, insertion, merge,
 * merge-insertion (threshold 25) and quick sort on random arrays.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXVAL 1000000
#define THRESHOLD 25
#define NUM_SIZES 6

/* ---------- timing ---------- */
long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

/* ---------- helpers ---------- */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randArray(int arr[], int size, int maxVal) {
    for (int i = 0; i < size; i++)
        arr[i] = rand() % maxVal + 1;
}

void arrayCopy(int from[], int to[], int size) {
    memcpy(to, from, size * sizeof(int));
}

/* ---------- selection sort ---------- */
void selectionSort(int arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}

/* ---------- bubble sort ---------- */
void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

/* ---------- insertion sort ---------- */
void insertionSort(int arr[], int n) {
    int i, item, j;
    for (i = 1; i < n; i++) {
        item = arr[i];
        for (j = i - 1; j >= 0; j--) {
            if (arr[j] > item)
                arr[j + 1] = arr[j];
            else
                break;
        }
        arr[j + 1] = item;
    }
}

/* ---------- merge sort ---------- */
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/* ---------- merge insertion sort ---------- */
/* insertion sort on the sub-array arr[l..r] */
void insertionSortRange(int arr[], int l, int r) {
    int i, item, j;
    for (i = l + 1; i <= r; i++) {
        item = arr[i];
        for (j = i - 1; j >= l; j--) {
            if (arr[j] > item)
                arr[j + 1] = arr[j];
            else
                break;
        }
        arr[j + 1] = item;
    }
}

/* merge sort that switches to insertion sort once the
 * sub-array size reaches the threshold (25) */
void mergeInsertionSort(int arr[], int l, int r) {
    if (r - l + 1 <= THRESHOLD) {
        insertionSortRange(arr, l, r);
        return;
    }
    int m = (l + r) / 2;
    mergeInsertionSort(arr, l, m);
    mergeInsertionSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

/* ---------- quick sort ---------- */
int partition(int vals[], int low, int high) {
    /* random pivot moved to the front */
    int i = low + rand() % (high - low + 1);
    swap(&vals[low], &vals[i]);

    int lowpos = low;
    low++;

    while (low <= high) {
        while (low <= high && vals[low] <= vals[lowpos]) low++;
        while (high >= low && vals[high] > vals[lowpos]) high--;
        if (low < high)
            swap(&vals[low], &vals[high]);
    }

    swap(&vals[lowpos], &vals[high]);
    return high;
}

void quickSort(int numbers[], int low, int high) {
    if (low < high) {
        int split = partition(numbers, low, high);
        quickSort(numbers, low, split - 1);
        quickSort(numbers, split + 1, high);
    }
}

/* ---------- main ---------- */
int main(void) {
    int sizes[NUM_SIZES] = {10000, 20000, 30000, 40000, 50000, 100000};
    long results[NUM_SIZES][6];
    const char *names[6] = {"Selection", "Bubble", "Insertion",
                            "Merge", "Merge Insertion", "Quick"};
    clock_t start, end;
    long elapsed;

    srand(time(NULL));

    for (int i = 0; i < NUM_SIZES; i++) {
        int *originalArray = (int *)malloc(sizes[i] * sizeof(int));
        int *sortedArray = (int *)malloc(sizes[i] * sizeof(int));
        randArray(originalArray, sizes[i], MAXVAL);

        /* Selection sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        selectionSort(sortedArray, sizes[i]);
        end = clock();
        elapsed = timediff(start, end);
        results[i][0] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Selection sort\n", sizes[i], elapsed);

        /* Bubble sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        bubbleSort(sortedArray, sizes[i]);
        end = clock();
        elapsed = timediff(start, end);
        results[i][1] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Bubble sort\n", sizes[i], elapsed);

        /* Insertion sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        insertionSort(sortedArray, sizes[i]);
        end = clock();
        elapsed = timediff(start, end);
        results[i][2] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Insertion sort\n", sizes[i], elapsed);

        /* Merge sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        mergeSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        results[i][3] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Merge sort\n", sizes[i], elapsed);

        /* Merge insertion sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        mergeInsertionSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        results[i][4] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Merge Insertion sort\n", sizes[i], elapsed);

        /* Quick sort */
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        quickSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        results[i][5] = elapsed;
        printf("\nsorting %d values takes %ld milli seconds for Quick sort\n", sizes[i], elapsed);

        free(originalArray);
        free(sortedArray);
    }

    /* summary table (milliseconds) for copying into Excel */
    printf("\n\nData Size");
    for (int a = 0; a < 6; a++)
        printf("\t%s", names[a]);
    printf("\n");
    for (int i = 0; i < NUM_SIZES; i++) {
        printf("%d", sizes[i]);
        for (int a = 0; a < 6; a++)
            printf("\t%ld", results[i][a]);
        printf("\n");
    }

    return 0;
}
