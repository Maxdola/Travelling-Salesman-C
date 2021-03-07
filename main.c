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

int countWords(const char *string, int stringLength) {
    int a = 0;
    for (int i = 0; i < stringLength - 1; i++) {
        if ((string[i] < 91 && string[i] > 64) || (string[i] < 123 && string[i] > 96) || string[i] == 45 ||
            string[i] == 95) {
            while ((string[i] < 91 && string[i] > 64) || (string[i] < 123 && string[i] > 96) || string[i] == 45 ||
                   string[i] == 95) {
                i++;
            }
            a++;
        }
    }
    return a;
}

void strToStrPtr(char **strPtr, const char *string, int stringLength) {
    int counter = 0;
    for (int i = 0; i < stringLength - 1; i++) {
        if ((string[i] < 91 && string[i] > 64) || (string[i] < 123 && string[i] > 96) || string[i] == 45 ||
            string[i] == 95) {
            int j = 0;
            while ((string[i] < 91 && string[i] > 64) || (string[i] < 123 && string[i] > 96) || string[i] == 45 ||
                   string[i] == 95) {
                strPtr[counter][j++] = string[i++];
            }
            strPtr[counter][j] = '\0';
            counter++;
        }
    }
}

int strToIntPtr(Distance *distances, const char *string, int city, int n) {
    int size = 1;
    int i = 0;
    while (string[i] != 0) {
        if (string[i] < 58 && string[i] > 47) {
            int value = 0;
            while (string[i] < 58 && string[i] > 47) {
                value = (value * 10) /*Pushes to the left to make room for the new number*/ +
                        (string[i++] - 48); //Places the new number to its position
            }
            Distance dist = {city, size - 1, value};
            distances[(city * n) + (size) - 1] = dist;
            size++;
        } else {
            i++;
        }
    }
    return size - 1;
}

FILE *getFile(char *path, char *mode) {
    return fopen(path, mode);
}

DistanceTable *parseData(FILE *file, DistanceTable *distanceTable) {
    int currentChar;
    char *currentLine = malloc(1 * sizeof(char));
    int sizeCL = 0;
    int lineNum = -1;
    int writtenDistances = 0;
    while ((currentChar = fgetc(file)) != EOF) {
        currentLine = realloc(currentLine, (++sizeCL) * sizeof(char));
        if (currentChar == '\n') {
            if (lineNum == -1) {
                char **strPtr = malloc(1 * sizeof(char));
                distanceTable->n = countWords(currentLine, sizeCL);
                strToStrPtr(distanceTable->cities, currentLine, sizeCL);
                free(strPtr);
            } else {
                int size = strToIntPtr(distanceTable->distance, currentLine, lineNum, distanceTable->n);
                if (size != distanceTable->n) {
                    printf("Table not correctly formatted!\n");
                    exit(0);
                }
                writtenDistances += size;
            }
            lineNum++;
            sizeCL = 1;
        }
        currentLine[sizeCL - 1] = (char) currentChar;
    }
    if (writtenDistances != distanceTable->n * distanceTable->n) {
        int size = strToIntPtr(distanceTable->distance, currentLine, lineNum, distanceTable->n);
        if (size != distanceTable->n) {
            printf("Table not correctly formatted!\n");
            exit(0);
        }
    }
    return NULL;
}

FILE *loadFilePrompt(char *mode) {
    int fileNameSize = 20;
    printf("Type your file name [limited to %d character]:\n", fileNameSize);
    char fileName[fileNameSize];
    printf("Filename:");
    scanf("%20s", fileName);
    while (getchar() != '\n');

    //FIXME remove when finishing the project
    char temp[24];
    temp[0] = temp[1] = '.';
    temp[2] = '/';
    for (int i = 3; i < 24; i++)
        temp[i] = fileName[i - 3];

    return getFile(temp, mode);
}

//TODO remove
void loadDataTxT(DistanceTable *distanceTable, const char *fileName) {
    FILE *fptr;

    //FIXME remove when finishing the project
    char temp[24];
    temp[0] = temp[1] = '.';
    temp[2] = '/';
    for (int i = 3; i < 24; i++)
        temp[i] = fileName[i - 3];

    fptr = getFile(temp, "r+");

    if (fptr == NULL) {
        printf("Error!\nFile could not be found, is protected or read-only!\n");
        return;
    }
    parseData(fptr, distanceTable);
    fclose(fptr);
    printf("%d Cities loaded!\n", distanceTable->n);
}

void readFile(DistanceTable *distanceTable, int cityLimit) {
    FILE *fptr = NULL;

    fptr = loadFilePrompt("r+");

    if (fptr == NULL) {
        printf("Error!\nFile could not be found, is protected or read-only!\n");
        return;
    }
    distanceTable->distance = malloc(cityLimit * cityLimit * sizeof(Distance));
    distanceTable->cities = malloc(cityLimit * sizeof(char *));
    for (int i = 0; i < cityLimit; i++) distanceTable->cities[i] = malloc(40 * sizeof(char *));
    distanceTable->n = 0;
    parseData(fptr, distanceTable);
    fclose(fptr);
    if (distanceTable->n == 0) {
        printf("No cities found. Table cleared");
        return;
    }
    printf("%d Cities successfully loaded!\n", distanceTable->n);
}

Distance *findDistance(DistanceTable *table, int from, int to) {
    int a = from * table->n + to;
    if (table->distance[a].from == from && table->distance[a].to == to) return &table->distance[a];
    for (int i = 0; i < table->n * table->n; ++i) {
        if (table->distance[i].from == from && table->distance[i].to == to) return &table->distance[i];
    }
    return NULL;
}

void saveTable(DistanceTable *table) {
    FILE *fptr;

    fptr = loadFilePrompt("w+");

    if (fptr == NULL) {
        printf("Failed to open requested file.\n");
        return;
    }

    Distance *distance = NULL;

    for (int i = 0; i <= table->n; ++i) {
        for (int j = 0; j < table->n; ++j) {
            if (i == 0) {
                fprintf(fptr, j == 0 ? "%s" : " %s", table->cities[j]);
            } else {

                distance = findDistance(table, i - 1, j);
                if (distance != NULL) {
                    fprintf(fptr, j == 0 ? "%d" : " %d", distance->dist);
                    distance = NULL;
                } else {
                    fprintf(fptr, "err");
                }
            }
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
}

void printTable(DistanceTable *table) {

    Distance *distance = NULL;

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

int cityNameIndex(DistanceTable *table, char *name) {
    for (int i = 0; i < table->n; ++i) {
        if (strcmp(table->cities[i], name) == 0) return i;
    }
    return -1;
}

void modifyTable(DistanceTable *table) {

    char from[40];
    int fromIndex = -1;

    char to[40];
    int toIndex = -1;

    printf("Please now enter the City names you want to modify:\n[maximal length is 40 characters]\n\n");
    printf("Cities:");
    for (int i = 0; i < table->n; ++i) {
        printf(" %s", table->cities[i]);
    }
    printf("\n\n");

    while (fromIndex == -1) {
        printf("First City: ");
        scanf("%40s", from);

        if ((fromIndex = cityNameIndex(table, from)) == -1) {
            printf("There is no city called: %s\n\n", from);
            memset(&from[0], 0, sizeof(from));
        } else {
            printf("\n");
        }
    }

    while (toIndex == -1) {
        printf("Second City: ");
        scanf("%40s", to);

        if ((toIndex = cityNameIndex(table, to)) == -1) {
            printf("There is no city called: %s\n\n", to);
            memset(&to[0], 0, sizeof(to));
        } else {
            printf("\n");
        }
    }

    Distance *atob = findDistance(table, fromIndex, toIndex);
    Distance *btoa = findDistance(table, toIndex, fromIndex);

    int newAtoB = -1, newBtoA = -1;

    printf("Current distances:\n");
    printf("%s -> %s = %d\n", from, to, atob->dist);
    printf("%s <- %s = %d\n", from, to, btoa->dist);

    printf("Please now enter the new distances:\n");

    while (newAtoB < 0) {
        printf("\n%s -> %s: ", from, to);
        scanf("%d", &newAtoB);
        while (getchar() != '\n');
    }

    while (newBtoA < 0) {
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

int recursiveHeuristicCalculation(DistanceTable *distanceTable, int *field, int *way, int size, int pos, int oldDistance) {
    if (size == pos) {
        way[pos] = way[0];
        return oldDistance + findDistance(distanceTable, way[pos - 1], way[pos])->dist;
    }
    int distance = -1;
    int newCity;
    for (int i = 0; i < size; i++) {
        if (field[i] != -1) {
            if (distance != -1) {
                int newDist = findDistance(distanceTable, way[pos - 1], field[i])->dist;
                if (newDist < distance) {
                    distance = newDist;
                    newCity = field[i];
                }
            } else {
                distance = findDistance(distanceTable, way[pos - 1], field[i])->dist;
                newCity = field[i];
            }
        }
    }
    field[newCity] = -1;
    way[pos] = newCity;
    return recursiveHeuristicCalculation(distanceTable, field, way, size, pos+1, distance+oldDistance);
}

void heuristicCalc(DistanceTable *distanceTable, int startCity) {
    int way[distanceTable->n + 1];
    int field[distanceTable->n];
    for (int i = 0; i < distanceTable->n; i++) field[i] = i;
    field[startCity] = -1;
    way[0] = startCity;
    int distance = recursiveHeuristicCalculation(distanceTable, field, way, distanceTable->n, 1, 0);
    printf("Route: %s", distanceTable->cities[startCity]);
    for (int i = 1; i < distanceTable->n + 1; i++) printf(" -%d> %s",findDistance(distanceTable, way[i - 1], way[i])->dist,distanceTable->cities[way[i]]);
    printf("\nDistance: %d\n", distance);
}

int main() {
    //DistanceTable Init
    DistanceTable table;
    int cityLimit = 50;
    table.distance = malloc(cityLimit * cityLimit * sizeof(Distance));
    table.cities = malloc(cityLimit * sizeof(char *));
    for (int i = 0; i < cityLimit; i++) table.cities[i] = malloc(40 * sizeof(char *));
    table.n = 0;

    int end = 0;
    while (end == 0) {
        int switchNum = -1;
        printf("-------------\nOperations:\n1. End Program\n2. Read File\n3. Display Table\n4. Modify Table\n5. Save Table\n6. Heuristic Calculation for the shortest Route\n\nType the number of your operation:");
        scanf("%d", &switchNum);
        while (getchar() != '\n');
        switch (switchNum) {
            case 6:
                if (table.n == 0) {
                    printf("Table not loaded!\n");
                    break;
                }
                for (int i = 0; i < table.n; i++) printf("%d. %s\n",i+1,table.cities[i]);
                printf("Enter the number of the City:");
                int cityNum = 0;
                scanf("%d", &cityNum);
                while (getchar() != '\n');
                if (cityNum < 1 || cityNum > table.n) break;
                heuristicCalc(&table,cityNum-1);
                break;
            case 5:
                if (table.n == 0) {
                    printf("Table not loaded.\n");
                    break;
                }
                saveTable(&table);
                break;
            case 4:
                if (table.n == 0) {
                    printf("Table not loaded.\n");
                    break;
                }
                modifyTable(&table);
                break;
            case 3:
                if (table.n == 0) {
                    printf("Table not loaded.\n");
                    break;
                }
                printTable(&table);
                break;
            case 2:
                readFile(&table, cityLimit);
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
