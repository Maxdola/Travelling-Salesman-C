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


//TODO implement selection between program options
int main() {
    FILE* fptr;
    fptr = getFile("../data.txt");

    if(fptr == NULL){
        printf("Error!");
        exit(1);
    }

    fclose(fptr);

    return 0;
}
