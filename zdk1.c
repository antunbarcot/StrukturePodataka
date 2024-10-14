//Napisati program koji prvo procita koliko redaka ima datoteka, 
//tj.koliko ima studenata zapisanih u datoteci. Nakon toga potrebno 
//je dinamicki alocirati prostor za niz struktura studenata(ime, prezime, bodovi) 
//i ucitati iz datoteke sve zapise.Na ekran ispisati ime, prezime, apsolutni i
//relativni broj bodova.Napomena: Svaki redak datoteke sadrzi ime i prezime studenta, 
//te broj bodova na kolokviju. Relativan_br_bodova = br_bodova / max_br_bodova * 100


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

int readNoRowsInFile() {

    int counter = 0;
    char buffer[MAX_LINE] = { 0 };

    FILE* filePointer = fopen("studenti.txt", "r");

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

    noRows = readNoRowsInFile();

    if (noRows > 0) {
        FILE* filePointer = fopen("studenti.txt", "r");
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

        printf("Ime\t\tPrezime\t\tAps.bodovi\tRel.bodovi\n");
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