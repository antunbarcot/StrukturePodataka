#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define MAX_POINTS 15
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct _student {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    double points;
} Student;

int readNoRowsInFile(const char* filename) {
    int counter = 0;
    char buffer[MAX_LINE] = { 0 };

    FILE* filePointer = fopen(filename, "r");
    if (!filePointer) {
        printf("File not opened!\n");
        return FILE_ERROR_OPEN;
    }

    while (fgets(buffer, MAX_LINE, filePointer) != NULL) {
        counter++;
    }

    fclose(filePointer);

    return counter;
}

int main() {
    int i = 0, noRows = 0;

    
    const char* filename = "C:\\Users\\Antun\\Desktop\\VS DEMO\\PrviZadatak\\bodovi.txt";

    noRows = readNoRowsInFile(filename);

    if (noRows > 0) {
        FILE* filePointer = fopen(filename, "r");
        if (!filePointer) {
            printf("File not opened!\n");
            return FILE_ERROR_OPEN;
        }

        Student* students = (Student*)malloc(noRows * sizeof(Student));
        if (students == NULL) {
            printf("Malloc error!\n");
            fclose(filePointer);
            return MALLOC_ERROR;
        }

        for (i = 0; i < noRows; i++) {
            if (fscanf(filePointer, " %s %s %lf ", students[i].name, students[i].surname, &students[i].points) != 3) {
                printf("Error reading student data!\n");
                free(students);
                fclose(filePointer);
                return SCANF_ERROR;
            }
        }

        printf("Ime\t\tPrezime\t\tApsolutni bodovi\tRelativni bodovi\n");
        for (i = 0; i < noRows; i++) {
            double relativePoints = (students[i].points / MAX_POINTS) * 100;
            printf("%s\t\t%s\t\t%.2lf\t\t%.2lf%%\n", students[i].name, students[i].surname, students[i].points, relativePoints);
        }

        fclose(filePointer);
        free(students);
    }
    else {
        printf("Datoteka je prazna ili nema podataka.\n");
    }

    return EXIT_SUCCESS;
}
