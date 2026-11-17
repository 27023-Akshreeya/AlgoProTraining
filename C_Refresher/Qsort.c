#include <stdio.h>
#include <stdlib.h>

int custom_compare_int(const void *a, const void *b){
    int valA = *(const int *)a;
    int valB = *(const int *)b;

    if(valA < valB) return -1;
    else if(valA > valB) return 1;
    return 0;
}

int custom_compare_char(const void *a, const void *b){
    char valA = *(const char *)a;
    char valB = *(const char *)b;

    if(valA < valB) return -1;
    else if(valA > valB) return 1;
    return 0;
}
int main()
{
    int arr[] = {2,5,1,3,4};
    int size = sizeof(arr)/sizeof(arr[0]);

    char arrch[] = {'d','e','a','b'};
    int size2 = sizeof(arrch)/sizeof(arrch[0]);

    qsort(arr,size, sizeof(arr[0]),custom_compare_int);
    printf("Sorting integer array: ");
    for (int i = 0; i<size ; i++){
        printf("%d",arr[i]);
    }

    qsort(arrch,size2, sizeof(arrch[0]), custom_compare_char);
    printf("\nsorting character array :");
    for(int i = 0; i< size2; i++){
        printf("%c", arrch[i]);
    }
    return 0;
}