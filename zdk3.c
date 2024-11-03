/*2. Definirati strukturu osoba(ime, prezime, godina rodenja) i napisati program koji :
A.dinamicki dodaje novi element na pocetak liste,
B.ispisuje listu,
C.dinamicki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.brise odredeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

/*3. Prethodnom zadatku dodati funkcije :
A.dinamicki dodaje novi element iza odredenog elementa,
B.dinamicki dodaje novi element ispred odredenog elementa,
C.sortira listu po prezimenima osoba
D.upisuje listu u datoteku,
E.cita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 1
#define EXIT_CODE 0
#define SCANF_ERROR -1
#define MALLOC_ERROR -2
#define FILE_ERROR_OPEN -3
#define MAX_LINE 1024

struct _person;
typedef struct _person* Position;

Position createPerson();
int addToFrontOfTheList(Position);
int addToEndOfTheList(Position);
int printList(Position);
int printPerson(Position);
Position findPerson(Position, char*);
Position findPersonPrev(Position, char*);
int deletePerson(Position, char*);
Position deleteList(Position);
int addAfter(Position, char*);
int fileWrite(Position);
int fileRead(Position);
int printFile();
int sortList(Position);


typedef struct _person {

	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;

}Person;

int main() {


	char input = 0;
	char surname[MAX_SIZE];

	FILE* fp = NULL; 

	Person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	Person headR = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };

	printf("a - add person to front of the list\nb - print list\nc - add person to end of the list\nd - find person with surname\n");
	printf("e - delete person with surname\nf - delete list\ng - add new person after person with surname\nh - add new person before person with surname\ni - sort alphabetically by surname\nj - write data into file\nk - read data from file ");

	do
	{
		printf("\nChoose action: ");
		if (scanf(" %c", &input) != 1) return SCANF_ERROR;
		input = tolower(input);

		switch (input) {

		case 'a':
			printf("Add to front of the list\n");
			addToFrontOfTheList(&head);
			break;

		case 'b':
			printf("Printing list:\n");
			printList(head.next);
			break;

		case 'c':
			printf("Add to end of the list\n");
			addToEndOfTheList(&head);
			break;

		case 'd':
			printf("Find person with surname: ");
			if (scanf(" %s", surname) != 1)return SCANF_ERROR;
			Position found = findPerson(head.next, surname);
			if (found) {
				printPerson(found);
			}
			else {
				printf("\tPerson with surname %s was not found.\n", surname);
			}
			break;

		case 'e':
			printf("Delete person with surname: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			deletePerson(&head, surname);
			break;

		case 'f':
			head.next = deleteList(head.next);

			if (head.next == NULL) {
				printf("List deleted successfulLy.\n");
			}
			break;

		case 'g':
			printf("Add new person after person with surname: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			addAfter(&head, surname);
			break;

		case 'h':
			printf("Add new person before person with surname: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			addBefore(&head, surname);
			break;

		case 'i':
			printf("Sorting alphabetically by surname: ");
			sortList(&head);
			printList(head.next);
			break;

		case 'j':
			printf("Writing data into file.\n");
			fileWrite(head.next);
			break;

		case 'k':
			printf("Reading data from file.\n");
			fileRead(&headR);
			printList(headR.next);
			
			break;

		case '0':
			break;

		default:
			printf("Invalid input. Try again.\n");
			break;

		}
	} while (input != 0);


	return  EXIT_CODE;

}
Position createPerson() {

	Position person = NULL;
	person = (Position)malloc(sizeof(Person));

	if (!person) {
		printf("Malloc error.\n");
		return MALLOC_ERROR;
	}

	printf("\tName: ");
	if (scanf(" %s", person->name) != 1) return SCANF_ERROR;

	printf("\tSurname: ");
	if (scanf(" %s", person->surname) != 1) return SCANF_ERROR;

	printf("\tBirth year: ");
	if (scanf(" %d", &person->birthYear) != 1) return SCANF_ERROR;

	person->next = NULL;
	return person;

}

int printPerson(Position person) {

	if (person != NULL) {

		printf("Name: %s\n", person->name);
		printf("Surname: %s\n", person->surname);
		printf("Birth Year: %d\n", person->birthYear);
	}

	return EXIT_SUCCESS;

}

int printList(Position person) {

	if (person == NULL) {
		printf("Empty list.");
		return EXIT_SUCCESS;
	}

	while (person != NULL) {
		printPerson(person);
		person = person->next;

	}
	return EXIT_SUCCESS;
}

int addToFrontOfTheList(Position head) {  


	Position newPerson = NULL;
	newPerson = createPerson();

	if (!newPerson) {
		printf("Malloc error.\n");
		return MALLOC_ERROR;
	}

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int addToEndOfTheList(Position head) {

	Position newPerson = createPerson();

	if (!newPerson) {

		printf("Malloc error.");
		return MALLOC_ERROR;

	}

	while (head->next != NULL) {

		head = head->next;
	}

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;

}

Position findPerson(Position current, char* sur) {

	while (current != NULL && strcmp(current->surname, sur))
		current = current->next;

	return current;
}

Position findPersonPrev(Position current, char* sur) {

	while (current->next != NULL && strcmp(current->next->surname, sur))
		current = current->next;

	if (current->next == NULL) return NULL;

	return current;

}

int deletePerson(Position head, char* sur) {

	Position temp = NULL;
	Position prev = head;

	while (prev->next != NULL && strcmp(prev->next->surname, sur) != 0) {
		prev = prev->next;
	}

	if (prev->next == NULL) {
		printf("Person with surname %s was not found.\n", sur);
		return EXIT_SUCCESS;
	}

	temp = prev->next;
	prev->next = temp->next;
	free(temp);

	printf("Person deleted.\n");


	return EXIT_SUCCESS;

}

Position deleteList(Position head) {

	Position temp = head;

	while (temp != NULL) {

		temp = temp->next;
		free(head);
		head = temp;
	}
	return head;
}


int addAfter(Position head, char* sur) {

	Position newPerson = NULL;
	Position personWithSurname = NULL;

	personWithSurname = findPerson(head, sur);

	if (personWithSurname != NULL) {

		newPerson = createPerson();

		if (!newPerson) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;

		}

		newPerson->next = personWithSurname->next;
		personWithSurname->next = newPerson;

	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;

}

int addBefore(Position head, char* sur) {

	Position newPerson = NULL;
	Position personBeforeSurname = NULL;

	personBeforeSurname = findPersonPrev(head, sur);

	if (personBeforeSurname != NULL) {

		newPerson = createPerson();

		if (!newPerson) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;

		}

		newPerson->next = personBeforeSurname->next;
		personBeforeSurname->next = newPerson;

	}
	else ("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;

}

int fileWrite(Position current) {


	FILE* fp = NULL;
	fp = fopen("persons.txt", "w");

	if (fp == NULL) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	while (current != NULL) {

		fprintf(fp, "%s %s %d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}

	fclose(fp);
	printf("\tDone.\n");

	return EXIT_SUCCESS;
}

int printFile() {


	char buffer[MAX_LINE];
	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");

	if (!fp) {
		printf("Unable to open the file.\n");
		return FILE_ERROR_OPEN;
	}

	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
	return EXIT_SUCCESS;
}


int fileRead(Position head) {

	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");

	if (!fp) {
		printf("Unable to open the file.\n");
		fclose(fp);
		return FILE_ERROR_OPEN;
	}

	Position prev = NULL;
	prev = head;
	Position current = NULL;

	deleteList(head->next);
	

	while (!feof(fp)) {

		current = (Position)malloc(sizeof(Person));
		if (!current) {
			printf("Malloc error.\n");
			fclose(fp);
			return MALLOC_ERROR;
		}

		if (fscanf(fp, "%s %s %d", current->name, current->surname, &current->birthYear) != 3) {
			free(current);
			break;
		}

		current->next = prev->next;
		prev->next = current;

		prev = current;

	}
	
	fclose(fp);
	
	return EXIT_SUCCESS;

}

int sortList(Position head) {

	if (head == NULL || head->next == NULL) {
		printf("List empty or one element only.\n");
		return EXIT_SUCCESS;

	}

	Position i, j, prevJ, temp, end = NULL;

	while (head->next != end) {

		prevJ = head;
		j = head->next;

		while (j->next != end) {

			if (strcmp(j->surname, j->next->surname) > 0) {

				temp = j->next;
				j->next = temp->next;
				temp->next = j;
				prevJ->next = temp;


				j = temp;
			}

			prevJ = j;
			j = j->next;


		}

		end = j;
	}
	printf("List sorted alphabetically by surname.\n");
	return EXIT_SUCCESS;

	}
