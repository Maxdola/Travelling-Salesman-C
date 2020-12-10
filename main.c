#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int from;
    int to;
    int dist;
} Distance;

typedef struct {
    int n;
    char **cities;
    Distance *distance;
} DistanceTable;

//FIXME is null when file is protected/read-only (handle Null Error somehow)
FILE* getFile(char* path) {
    return fopen(path, "r+");
}

void parseData(FILE* file) {

}

void readFile() {
    FILE* fptr;
    fptr = getFile("../data.txt");

    if(fptr == NULL){
        printf("Error!");
        exit(1);
    }

    fclose(fptr);
}

int main() {
    int end = 0;
    int switchNum = -1;
    while (end == 0) {
        printf("-------------\nOperations:\n1. End Program\n\nType the number of your operation:");
        scanf("%d", &switchNum);
        while ( getchar() != '\n' );
        switch (switchNum) {
            case 1:
                printf("Ending Program...\n");
                end = 1;
                break;
            case -1:
                printf("This Input was not valid.\n");
                break;
            default:
                printf("This Operation does not exist.\n");
                break;
        }
        switchNum = -1;
    }
    return 0;
}
