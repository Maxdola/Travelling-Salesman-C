#include <stdio.h>
#include <stdlib.h>

void test(int *arr) {
    free(arr);
    arr = malloc(1*sizeof(int));
    arr[0] = 12;
    arr = realloc(arr, 4*sizeof(int));
    arr[1] = 3;
    arr[2] = 1;
    arr[3] = 53;
}

int maini() {
    int *other = malloc(1*sizeof(int));

    test(other);

    printf("%d",other[3]);

    free(other);
    return 0;
}
