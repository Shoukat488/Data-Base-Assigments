#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int MAX;
int THREAD_MAX;

// array
int *a;

// thread control parameters
struct task
{
    int tsk_no;
    int tsk_low;
    int tsk_high;
};

void merge(int low, int mid, int high)
{

    int n1 = mid - low + 1;
    int n2 = high - mid;

    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    int i;
    int j;

    for (i = 0; i < n1; i++)
        left[i] = a[i + low];

    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;

    i = j = 0;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    while (i < n1)
        a[k++] = left[i++];

    while (j < n2)
        a[k++] = right[j++];

    free(left);
    free(right);
}

void merge_sort(int low, int high)
{

    int mid = low + (high - low) / 2;

    if (low < high)
    {
        merge_sort(low, mid);

        merge_sort(mid + 1, high);

        merge(low, mid, high);
    }
}

void *
merge_sort_controlller(void *arg)
{
    struct task *task = arg;
    int low;
    int high;

    low = task->tsk_low;
    high = task->tsk_high;

    int mid = low + (high - low) / 2;

    if (low < high)
    {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}

int main()
{

    MAX = 50000;
    THREAD_MAX = 4;
    struct task *task;
    struct task tsklist[THREAD_MAX];
    pthread_t threads[THREAD_MAX];
    clock_t start, end, final;
    double timeTaken;
    FILE *file;
    // allocate the array
    a = malloc(sizeof(int) * MAX);
    start = clock();
    file = fopen("50000N.txt", "r");

    for (int i = 0; i < MAX; i++)
    {
        fscanf(file, "%d", &a[i]);
    }

    int len = MAX / THREAD_MAX;

    int low = 0;

    for (int i = 0; i < THREAD_MAX; i++, low += len)
    {
        task = &tsklist[i];
        task->tsk_no = i;

        task->tsk_low = low;
        task->tsk_high = low + len - 1;
        if (i == (THREAD_MAX - 1))
            task->tsk_high = MAX - 1;
    }

    // creating 4 threads
    for (int i = 0; i < THREAD_MAX; i++)
    {
        task = &tsklist[i];
        pthread_create(&threads[i], NULL, merge_sort_controlller, task);
    }

    // joining all 4 threads
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_join(threads[i], NULL);

    // merging the final 4 parts
    struct task *tskm = &tsklist[0];
    for (int i = 1; i < THREAD_MAX; i++)
    {
        struct task *task = &tsklist[i];
        merge(tskm->tsk_low, task->tsk_low - 1, task->tsk_high);
    }

    // displaying sorted array
    // printf("\n\nSorted array:");
    // for (int i = 0; i < MAX; i++)
    //     printf(" %d", a[i]);
    // printf("\n");

    end = clock();
    final = end - start;
    timeTaken = ((double)(final) / CLOCKS_PER_SEC);
    printf("Total time: %f", timeTaken);

    return 0;
}