#include <stdio.h>
#include <stdlib.h>

int maini() {
    const int bufSize = 32;
    char **splitStrings;

    splitStrings = malloc(bufSize*sizeof(char*));

    for (int i=0;i<bufSize;i++){
        splitStrings[i]=malloc(bufSize*sizeof(char*));
    }

    splitStrings[0][0] = 'a';
    splitStrings[0][1] = '\0';

    printf("testchar: %c\n", splitStrings[0][0]);
    printf("teststr: %s\n", splitStrings[0]);

    free(splitStrings);

    return 0;
}
