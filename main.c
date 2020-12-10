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

FILE *getFile(char *path) {
    return fopen(path, "r+");
}

DistanceTable* parseData(FILE *file) {
    return NULL;
}

DistanceTable *readFile() {
    FILE *fptr;

    int fileNameSize = 20;
    printf("Type your file name [limited to %d character]:\n", fileNameSize);
    char fileName[fileNameSize];
    scanf("%20s", fileName);
    while (getchar() != '\n');

    //FIXME remove when finishing the project
    char temp[24];
    temp[0] = temp[1] = '.';
    temp[2] = '/';
    for (int i = 3; i < 24; i++)
        temp[i] = fileName[i - 3];

    //FIXME change "temp" back to "fileName"
    fptr = getFile(temp);

    if (fptr == NULL) {
        printf("Error!\nFile could not be found, is protected or read-only!\n");
        return NULL;
    }
    DistanceTable* table = parseData(fptr);
    if (table == NULL) {
        printf("Error!\nFile not correctly formatted!\n");
    }
    fclose(fptr);
    return table;
}

void printTable(DistanceTable* table) {

}

int main() {
    DistanceTable* table = NULL;
    int end = 0;
    while (end == 0) {
        int switchNum = -1;
        printf("-------------\nOperations:\n1. End Program\n2. Read File\n3. Display Table\n\nType the number of your operation:");
        scanf("%d", &switchNum);
        while (getchar() != '\n');
        switch (switchNum) {
            case 3:
                if (table == NULL) {
                    printf("Table not loaded.\n");
                    break;
                }
                printTable(table);
                break;
            case 2:
                table = readFile();
                break;
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
    }
    return 0;
}
