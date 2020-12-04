#include <stdio.h>

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

void readFile();

int main() {
    readFile();
    return 0;
}

void readFile() {
    FILE *data;
    data = fopen("Data.txt", "r");
    //Code here
    fclose(data);
}