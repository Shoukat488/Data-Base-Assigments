#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX_SIZE 200000

void print_list(int *x, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", x[i]);
    }
}

void merge(int *X, int n, int *tmp)
{
    int i = 0;
    int j = n / 2;
    int ti = 0;

    while (i < n / 2 && j < n)
    {
        if (X[i] < X[j])
        {
            tmp[ti] = X[i];
            ti++;
            i++;
        }
        else
        {
            tmp[ti] = X[j];
            ti++;
            j++;
        }
    }
    while (i < n / 2)
    {
        tmp[ti] = X[i];
        ti++;
        i++;
    }
    while (j < n)
    {
        tmp[ti] = X[j];
        ti++;
        j++;
    }
    memcpy(X, tmp, n * sizeof(int));

} // end of merge()

void mergeSort(int *X, int n, int *tmp)
{
    if (n < 2)
        return;

#pragma omp task firstprivate(X, n, tmp)
    mergeSort(X, n / 2, tmp);

#pragma omp task firstprivate(X, n, tmp)
    mergeSort(X + (n / 2), n - (n / 2), tmp);

#pragma omp taskwait
    merge(X, n, tmp);
}

int main()
{

    int data[MAX_SIZE], tmp[MAX_SIZE];

    FILE *file;
    file = fopen("200000N.txt", "r");
    int i;
    for (i = 0; i < MAX_SIZE; i++)
    {
        fscanf(file, "%d", &data[i]);
    }
    //    printf("List Before Sorting...\n");
    //    print_list(data, n);
    double startTime = omp_get_wtime();
    mergeSort(data, MAX_SIZE, tmp);
    double endTime = omp_get_wtime();
    //    printf("\nList After Sorting...\n");
    // print_list(data, MAX_SIZE);
    printf("\nTime: %f\n", endTime - startTime);
}
