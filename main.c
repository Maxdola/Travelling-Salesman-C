#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

FILE* loadFilePrompt() {
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

    return getFile(temp);
}

void loadDataTxT(DistanceTable* distanceTable) {
    FILE *fptr;

    char fileName[] = "data.txt";

    //FIXME remove when finishing the project
    char temp[24];
    temp[0] = temp[1] = '.';
    temp[2] = '/';
    for (int i = 3; i < 24; i++)
        temp[i] = fileName[i - 3];

    fptr = getFile(temp);

    if (fptr == NULL) {
        printf("Error!\nFile could not be found, is protected or read-only!\n");
        return;
    }
    parseData(fptr, distanceTable);
    fclose(fptr);
    printf("Data fully loaded!\n");
}

void readFile(DistanceTable* distanceTable) {
    FILE *fptr;

    fptr = loadFilePrompt();

    if (fptr == NULL) {
        printf("Error!\nFile could not be found, is protected or read-only!\n");
        return;
    }
    parseData(fptr, distanceTable);
    fclose(fptr);
    printf("Data fully loaded!\n");
}

void saveTable() {
    FILE *fptr;

    fptr = loadFilePrompt();


}

Distance* findDistance(DistanceTable* table, int from, int to) {
    for (int i = 0; i < table->n * table->n; ++i) {
        if (table->distance[i].from == from && table->distance[i].to == to) return &table->distance[i];
    }
    return NULL;
}

void printTable(DistanceTable* table) {

    Distance* distance = NULL;

    for (int i = 0; i <= table->n; ++i) {
        for (int j = 0; j <= table->n; ++j) {
            if (i == 0) {
                if (j == 0) {
                    printf("%-15s", "");
                    continue;
                }
                printf("%-15s", table->cities[j - 1]);
            } else {
               if (j == 0) {
                    printf("%-15s ", table->cities[i - 1]);
                    continue;
               }


               distance = findDistance(table, i - 1, j - 1);
               if (distance != NULL) {
                   printf("%-15d", distance->dist);
                   distance = NULL;
               } else {
                   printf("%-15s", "err");
               }
            }
        }
        printf("\n");

    }
}

int cityNameIndex(DistanceTable* table, char* name) {
    for (int i = 0; i < table->n; ++i) {
        if (strcmp(table->cities[i], name) == 0) return i;
    }
    return -1;
}

void modifyDistance(DistanceTable* table) {

    char from[40];
    int fromIndex = -1;

    char to[40];
    int toIndex = -1;

    printf("Please now enter the City names you want to modify:\n[maximal lenght is 40 characters]\n\n");
    printf("Cities:");
    for (int i = 0; i < table->n; ++i) {
        printf(" %s", table->cities[i]);
    }
    printf("\n\n");

    while(fromIndex == -1) {
        printf("First City: ");
        scanf("%40s", from);

        if ((fromIndex = cityNameIndex(table, from)) == -1) {
            printf("There is no city called: %s\n\n", from);
            memset(&from[0], 0, sizeof(from));
        } else {
            printf("\n");
        }
    }

    while(toIndex == -1) {
        printf("Second City: ");
        scanf("%40s", to);

        if ((toIndex = cityNameIndex(table, to)) == -1) {
            printf("There is no city called: %s\n\n", to);
            memset(&to[0], 0, sizeof(to));
        } else {
            printf("\n");
        }
    }

    Distance* atob = findDistance(table, fromIndex, toIndex);
    Distance* btoa = findDistance(table, toIndex, fromIndex);

    int newAtoB = -1, newBtoA = -1;

    printf("Current distances:\n");
    printf("%s -> %s = %d\n", from, to, atob->dist);
    printf("%s <- %s = %d\n", from, to, btoa->dist);

    printf("Please now enter the new distances:\n");

    while (newAtoB == -1 && newAtoB < 0) {
        printf("\n%s -> %s: ", from, to);
        scanf("%d", &newAtoB);
        while (getchar() != '\n');
    }

    while (newBtoA == -1 && newBtoA < 0) {
        printf("\n%s <- %s: ", from, to);
        scanf("%d", &newBtoA);
        while (getchar() != '\n');
    }

    printf("\nNew distances:\n");
    printf("%s -> %s = %d\n", from, to, newAtoB);
    printf("%s <- %s = %d\n", from, to, newBtoA);

    atob->dist = newAtoB;
    btoa->dist = newBtoA;

    //printTable(table);
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
    //DistanceTable Init
    DistanceTable table;
    int cityLimit = 50;
    table.distance = malloc(cityLimit*cityLimit*sizeof(Distance));
    table.cities = malloc(cityLimit*sizeof(char*));
    for(int i = 0;i < cityLimit;i++) table.cities[i] = malloc(40* sizeof(char*));
    table.n = 0;

    int end = 0;

    DistanceTable testTable;
    testTable.n = 3;
    char* names[3];
    names[0] = "Moin";
    names[1] = "Mei";
    names[2] = "Ster";
    testTable.cities = names;
    //testTable.distance = NULL;

    Distance d0_0 = {from: 0, to: 0, dist: 0};
    Distance d0_1 = {from: 0, to: 1, dist: 13};
    Distance d0_2 = {from: 0, to: 2, dist: 44};

    Distance d1_0 = {from: 1, to: 0, dist: 47};
    Distance d1_1 = {from: 1, to: 1, dist: 0};
    Distance d1_2 = {from: 1, to: 2, dist: 44};

    Distance d2_0 = {from: 2, to: 0, dist: 87};
    Distance d2_1 = {from: 2, to: 1, dist: 11};
    Distance d2_2 = {from: 2, to: 2, dist: 0};

    Distance distance[9];
    distance[0] = d0_0;
    distance[1] = d0_1;
    distance[2] = d0_2;
    distance[3] = d1_0;
    distance[4] = d1_1;
    distance[5] = d1_2;
    distance[6] = d2_0;
    distance[7] = d2_1;
    distance[8] = d2_2;

    testTable.distance = distance;

    printTable(&testTable);
    loadDataTxT(&table);
    printTable(&table);
    //modifyDistance(&testTable);

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
