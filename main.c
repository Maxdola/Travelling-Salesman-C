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

int countWords(const char* string, int stringLength) {
    int a = 0;
    for (int i = 0; i < stringLength-1; i++) {
        if ((string[i] < 91 && string[i] > 64)||(string[i] < 123 && string[i] > 96) || string[i] == 45 || string[i] == 95) {
            while ((string[i] < 91 && string[i] > 64)||(string[i] < 123 && string[i] > 96) || string[i] == 45 || string[i] == 95) {
                i++;
            }
            a++;
        }
    }
    return a;
}

void strToStrPtr(char** strPtr, const char* string, int stringLength) {
    int counter = 0;
    for (int i = 0; i < stringLength-1; i++) {
        if ((string[i] < 91 && string[i] > 64)||(string[i] < 123 && string[i] > 96) || string[i] == 45 || string[i] == 95) {
            int j = 0;
            while ((string[i] < 91 && string[i] > 64)||(string[i] < 123 && string[i] > 96) || string[i] == 45 || string[i] == 95) {
                strPtr[counter][j++] = string[i++];
            }
            strPtr[counter][j] = '\0';
            counter++;
        }
    }
}

int strToIntPtr(Distance* distances, const char* string, int city, int n) {
    int size = 1;
    int i = 0;
    while(string[i] != 0) {
        if (string[i] < 58 && string[i] > 47) {
            int value = 0;
            while (string[i] < 58 && string[i] > 47) {
                value = (value * 10) /*Pushes to the left to make room for the new number*/ +
                        (string[i++] - 48); //Places the new number to its position
            }
            Distance dist = {city,size-1,value};
            distances[(city*n)+(size)-1] = dist;
            size++;
        } else {
            i++;
        }
    }
    return size-1;
}

FILE *getFile(char *path) {
    return fopen(path, "r+");
}

DistanceTable* parseData(FILE *file, DistanceTable* distanceTable) {
    int currentChar;
    char* currentLine = malloc(1 * sizeof(char));
    int sizeCL = 0;
    int lineNum = -1;
    while ((currentChar = fgetc(file)) != EOF)
    {
        currentLine = realloc(currentLine,(++sizeCL) * sizeof(char));
        if (currentChar == 10) {
            if (lineNum == -1) {
                char** strPtr = malloc(1* sizeof(char));
                distanceTable->n = countWords(currentLine,sizeCL);
                strToStrPtr(distanceTable->cities,currentLine,sizeCL);
                free(strPtr);
            } else {
                int size = strToIntPtr(distanceTable->distance, currentLine, lineNum, distanceTable->n);
                if (size != distanceTable->n) {
                    printf("Table not correctly formatted!\n");
                    exit(0);
                }
            }
            lineNum++;
            sizeCL = 1;
        }
        currentLine[sizeCL-1] = (char) currentChar;
    }
    return NULL;
}

void readFile(DistanceTable* distanceTable) {
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
    }
    parseData(fptr, distanceTable);
    fclose(fptr);
    printf("Data fully loaded!\n");
}

void printTable(DistanceTable* table) {

}

int main() {
    //DistanceTable Init
    DistanceTable table;
    int cityLimit = 50;
    table.distance = malloc(cityLimit*cityLimit*sizeof(Distance));
    table.cities = malloc(cityLimit*sizeof(char*));
    for(int i = 0;i < cityLimit;i++) table.cities[i] = malloc(40* sizeof(char*));
    table.n = 0;

    int end = 0;
    while (end == 0) {
        int switchNum = -1;
        printf("-------------\nOperations:\n1. End Program\n2. Read File\n3. Display Table\n\nType the number of your operation:");
        scanf("%d", &switchNum);
        while (getchar() != '\n');
        switch (switchNum) {
            case 3:
                if (table.n == 0) {
                    printf("Table not loaded.\n");
                    break;
                }
                printTable(&table);
                break;
            case 2:
                readFile(&table);
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
