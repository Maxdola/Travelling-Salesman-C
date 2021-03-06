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

int testing(int* values, const char* string, int overwrite) {
    int size = (sizeof(&values) / sizeof(int));
    int i = 0;
    while(string[i] != 0) {
        if (string[i] < 58 && string[i] > 47) {
            int value = 0;
            while (string[i] < 58 && string[i] > 47) {
                value = (value * 10) /*Pushes to the left to make room for the new number*/ +
                        (string[i++] - 48); //Places the new number to its position
            }
            if (overwrite == 0) values = realloc(values, (++size) * sizeof(int));
            else overwrite = 0;
            values[size-1] = value;
            //printf("%d\n", value);
        } else {
            i++;
        }
    }
    return size;
}

int main() {
    int* ptr = malloc(1 * sizeof(int));
    int size = testing(ptr,"11053 25490  23 1224 5 12", 1);
    for(int i = 0; i < size; i++) printf("%d\n",ptr[i]);
    free(ptr);
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
