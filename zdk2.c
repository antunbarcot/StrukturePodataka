/*2. Definirati strukturu osoba(ime, prezime, godina rodenja) i napisati program koji :
A.dinamicki dodaje novi element na pocetak liste,
B.ispisuje listu,
C.dinamicki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.brise odredeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 1
#define SCANF_ERROR -1
#define MALLOC_ERROR -2

struct _person;
typedef struct _person*Position;

Position createPerson();
int addToFrontOfTheList(Position);
int addToEndOfTheList(Position);
int printList(Position); 
int printPerson(Position);
Position findPerson(Position, char*);
int deletePerson(Position, char*);

typedef struct _person {

	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;

}Person;

int main() {


	char input = 0;
	char surname[MAX_SIZE];

	Person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };

	printf("a - add person to front of the list\nb - print list\nc - add person to end of the list\nd - find person with surname\ne - delete person with surname\n ");


	do
	{
		printf("\nChoose action: ");
		if (scanf(" %c", &input) != 1) return SCANF_ERROR;


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

		case '0':
			break;

		default:
			printf("Invalid input. Try again.\n");
			break;

		}
	} while (input != 0);

		return  EXIT_SUCCESS;
	
}
Position createPerson() {

	Position person = NULL;
	person = (Position)malloc(sizeof(Person));

	if (!person) {
		printf("Malloc error.\n");
		return MALLOC_ERROR;

	}
	
	printf("\tName: ");
	if (scanf (" %s", person -> name) != 1 ) return SCANF_ERROR;

	printf("\tSurname: ");
	if (scanf(" %s", person->surname) != 1) return SCANF_ERROR;

	printf("\tBirth year: ");
	if (scanf (" %d", &person->birthYear) != 1) return SCANF_ERROR;

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

int printList(Position head) {

	if (head == NULL) {
		printf("Empty list."); 
		return EXIT_SUCCESS;
	}

	while (head != NULL) {
		printPerson(head); 
		head=head->next; 

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

	while (current != NULL) {
		if (strcmp(current->surname, sur) == 0) {
			return current;
		}
		current = current->next;

	}

	return NULL; 
}

int deletePerson(Position head, char *sur) {

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

