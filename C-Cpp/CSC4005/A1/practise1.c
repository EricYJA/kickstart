#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

int swap(int *num, int i, int j);
void oddEvenSort(int arr[], int n);

int main (int argc, char ** argv) {
    int size = 100;
    int mykeys[size];
    int i;

    srand(48365);
    printf("Random Numbers: ");
    for(i=0; i<size; i++){
        mykeys[i]=((int)rand())%10000; /*生成随机数，并输出*/
        printf("%d ", mykeys[i]);
    }
    printf("\n");

    oddEvenSort(mykeys, size);

    printf("Sort outputs ");
    for (i = 0; i < size; i++){
        printf("%d ", mykeys[i]);
    }
    printf("\n");

    return 0;
}

void oddEvenSort(int arr[], int n) {
    int isSorted = 0; // Initially array is unsorted

    while (isSorted == 0)
    {
        isSorted = 1;

        // Perform Bubble sort on odd indexed element
        for (int i=1; i<=n-2; i=i+2)
        {
            if (arr[i] > arr[i+1])
             {
                swap(arr, i, i+1);
                isSorted = 0;
              }
        }

        // Perform Bubble sort on even indexed element
        for (int i=0; i<=n-2; i=i+2)
        {
            if (arr[i] > arr[i+1])
            {
                swap(arr, i, i+1);
                isSorted = 0;
            }
        }
    }

    return;
}

int swap(int *num, int i, int j) {
    int tmp = num[i];
    num[i] = num[j];
    num[j] = tmp;

    return 0;
}
