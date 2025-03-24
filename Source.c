#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Car Car;
typedef struct Node Node;

struct Car {
    int year;
    char* manufacturer;
    float cc;
};

struct Node {
    Car car;
    Node* next;
};

void insertAtBeginning(Node** start, Car car) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Eroare alocare memorie!\n");
        exit(1);
    }

    newNode->car = car;
    newNode->next = *start;
    *start = newNode;
}

void insertAtEnd(Node** start, Car car) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Eroare alocare memorie!\n");
        exit(1);
    }

    newNode->car = car;
    newNode->next = NULL;

    if (*start) {
        Node* aux = *start;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = newNode;
    }
    else {
        *start = newNode;
    }
}

Car deleteFromBeginning(Node** start) {
    if (*start == NULL) {
        printf("Lista este goală, nu se poate șterge nimic!\n");
        exit(1);
    }

    Node* deleted = *start;
    *start = (*start)->next;

    Car deletedCar = deleted->car;
    free(deleted);

    return deletedCar;
}

Car deleteFromEnd(Node** start) {
    if (*start == NULL) {
        printf("Lista este goala, nu se poate sterge nimic!\n");

    }
    Node* current = *start;
    Node* prev = NULL;
    while (current->next) {
        prev = current;
        current = current->next;
    }
    Car deletedCar = current->car;
    if (prev) {
        prev->next = NULL;
    }
    else {
        *start = NULL;
    }
    free(current);
    return deletedCar;
}

void deleteManufacturer(Node** start, char* manufacturer) {
    Node* prev = NULL;
    Node* current = *start;

    while (current != NULL) {
        if (strcmp(current->car.manufacturer, manufacturer) == 0) {
            Node* toDelete = current;
            if (prev != NULL) {
                prev->next = current->next;
            }
            else {
                *start = current->next;
            }

            free(toDelete->car.manufacturer);
            free(toDelete);

            if (prev) {
                current = prev->next; 
            }
            else {
                current = *start; 
            }
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

void printCar(Car car) {
    printf("%s from %d has %5.2f\n", car.manufacturer, car.year, car.cc);
}

void parseListAndPrint(Node* start) {
    while (start) {
        printCar(start->car);
        start = start->next;
    }
}

Car readCarFromFile(FILE* f) {
    char buffer[LINESIZE];

    if (!fgets(buffer, LINESIZE, f)) {
        printf("Eroare citire fișier!\n");
        exit(1);
    }

    Car car;
    char* carData = strtok(buffer, ",");
    if (!carData) exit(1);
    car.year = atoi(carData);

    carData = strtok(NULL, ",");
    if (!carData) exit(1);
    car.manufacturer = malloc((strlen(carData) + 1) * sizeof(char));
    if (!car.manufacturer) exit(1);
    strcpy(car.manufacturer, carData);

    carData = strtok(NULL, ",");
    if (!carData) exit(1);
    car.cc = atof(carData);

    return car;
}

void readCarsFromFile(Node** start, int* noCars, const char* fileName) {
    FILE* f = fopen(fileName, "r");
    if (!f) {
        printf("Eroare deschidere fișier!\n");
        exit(1);
    }

    char buffer[LINESIZE];
    if (!fgets(buffer, LINESIZE, f)) {
        printf("Eroare citire fișier!\n");
        fclose(f);
        exit(1);
    }

    *noCars = (int)strtol(buffer, NULL, 10);
    for (int i = 0; i < (*noCars); i++) {
        insertAtEnd(start, readCarFromFile(f));
    }

    fclose(f);
}

int main() {
    int noCars = 0;
    Node* list = NULL;

    readCarsFromFile(&list, &noCars, "cars.txt");
    parseListAndPrint(list);

    printf("-------------------\n");

    Car deletedCar = deleteFromBeginning(&list);
    printf("Dupa stergerea primului element:\n");
    parseListAndPrint(list);
    free(deletedCar.manufacturer);
    printf("-------------------\n");


    deleteManufacturer(&list, "Suzuki");
    printf("Dupa stergerea Suzuki:\n");
    parseListAndPrint(list);
    printf("-------------------\n");


    deletedCar = deleteFromEnd(&list);
    printf("Dupa stergerea ultimului element:\n");
    parseListAndPrint(list);
    free(deletedCar.manufacturer);
    printf("-------------------\n");

    
    return 0;
}
