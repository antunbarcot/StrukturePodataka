#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct article;
typedef struct article* ArticlePosition;
typedef struct article {
    char name[20];
    int quantity;
    int price;
    ArticlePosition next;
} Article;

struct receipt;
typedef struct receipt* ReceiptPosition;
typedef struct receipt {
    char date[10];
    Article headArticle;
    ReceiptPosition next;
} Receipt;

int dodajArtikl(ArticlePosition head, ArticlePosition article);
int dodajRacun(ReceiptPosition head, ReceiptPosition receipt);
int citajRacun(char* fileName, ReceiptPosition headReceipt);
int ispisiArtikle(ArticlePosition article);
int ispisiRacune(ReceiptPosition receipt);
int pronadiArtiklUPeriodu(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);
int obrisiArtikle(ArticlePosition articleHead);
int obrisiRacune(ReceiptPosition receiptHead);

int main()
{
    int income = 0, count = 0;
    char article[100] = { 0 };
    char startDate[20] = { 0 }, endDate[20] = { 0 };
    char currReceipt[MAX_LINE] = { 0 };
    FILE* filePointer = NULL;
    Receipt headReceipt = {
        .date = {0},
        .headArticle = NULL,
        .next = NULL
    };

    /*filePointer = fopen("racuni.txt", "r");
    if (filePointer == NULL) {
        printf("File could not be opened.\n");
        return FILE_OPEN_ERROR;
    }

    while (!feof(filePointer)) {
        fgets(currReceipt, MAX_LINE, filePointer);
        currReceipt[strcspn(currReceipt, "\n")] = 0;
        printf("%s\n", currReceipt);
        citajRacun(currReceipt, &headReceipt);
    }*/


    filePointer = fopen("racuni.txt", "r");
    if (!filePointer) {
        printf("Nije moguce otvoriti datoteku.\n");
        return FILE_OPEN_ERROR;
    }

    while (fgets(currReceipt, MAX_LINE, filePointer)) {
        currReceipt[strcspn(currReceipt, "\n")] = '\0'; // funkcija ce staviti \0 na mjesto \n od fgetsa i zavrsiti string

    int varijabla = 1;

    for (int i = 0; currReceipt[i] != '\0'; i++) {                      //preskok linije gdje je tab ili razmak
            if (currReceipt[i] != ' ' && currReceipt[i] != '\t') {
                varijabla = 0;
                break;
            }
    }
    if (varijabla) continue;

        
    if (citajRacun(currReceipt, &headReceipt) != 0) {
            fclose(filePointer);
            return FILE_OPEN_ERROR;
        }
    }
    fclose(filePointer);

    ispisiRacune(headReceipt.next);


    printf("\n");
    printf("Unesi naziv artikla: ");
    if (scanf("%s", article) != 1) return SCANF_ERROR;
    printf("Unesi pocetni datum (GGGG-MM-DD): ");
    if (scanf("%s", startDate) != 1) return SCANF_ERROR;
    printf("Unesi krajnji datum(GGGG-MM-DD): ");
    if (scanf("%s", endDate) != 1) return SCANF_ERROR;

    pronadiArtiklUPeriodu(&headReceipt, article, startDate, endDate, &income, &count);
    printf("Zarada od %d %s artikala: %d\n", count, article, income);

    obrisiRacune(&headReceipt);
    fclose(filePointer);

    return 0;
}

int dodajArtikl(ArticlePosition head, ArticlePosition article)
{
    ArticlePosition current = head;

    while (current->next != NULL && strcmp(current->next->name, article->name) == -1)   // sortiranje po nazivu artikla (abcd)
        current = current->next;

    article->next = current->next;
    current->next = article;

    return 0;
}

int dodajRacun(ReceiptPosition head, ReceiptPosition receipt)
{
    ReceiptPosition current = head;

    while (current->next != NULL && strcmp(current->next->date, receipt->date) <= 0)  // sortiranje računa po njihovom datumu
        current = current->next;

    receipt->next = current->next;
    current->next = receipt;

    return 0;
}

int citajRacun(char* fileName, ReceiptPosition headReceipt)
{
    char dateString[20] = { 0 };
    char articleString[MAX_LINE] = { 0 };
    int count = 0;
    FILE* filePointer = NULL;
    ReceiptPosition newReceipt = NULL;
    ArticlePosition newArticle = NULL;

    filePointer = fopen(fileName, "r");
    if (filePointer == NULL) {
        printf("Nije moguce otvoriti datoteku.\n");
        return FILE_OPEN_ERROR;
    }

    newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
    if (newReceipt == NULL) {
        printf("Malloc error.\n");
        return MALLOC_ERROR;
    }

    fgets(dateString, 20, filePointer);
    dateString[strcspn(dateString, "\n")] = 0;

    strcpy(newReceipt->date, dateString);
    newReceipt->headArticle.next = NULL;

    while (!feof(filePointer)) {
        newArticle = (ArticlePosition)malloc(sizeof(Article));
        if (newArticle == NULL) {
            printf("Malloc error.\n");
            return MALLOC_ERROR;
        }

        if (fscanf(filePointer, "%[^,], %d, %d ", newArticle->name, &newArticle->quantity, &newArticle->price) != 3) {  //%[^,] čita do zareza, ne može %s
            printf("fscanf error.\n");
            return SCANF_ERROR;
        }

        dodajArtikl(&newReceipt->headArticle, newArticle);
    }

    dodajRacun(headReceipt, newReceipt);

    fclose(filePointer);

    return 0;
}

int ispisiArtikle(ArticlePosition current)
{
    while (current != NULL) {

        printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
        current = current->next;
    }

    return 0;
}

int ispisiRacune(ReceiptPosition current)
{
    while (current != NULL) {
        printf("Racun od datuma %s:\n", current->date);

        ispisiArtikle(current->headArticle.next);
        current = current->next;
    }

    return 0;
}

int pronadiArtiklUPeriodu(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count)
{
    ArticlePosition currentArticle = NULL;

    while (currentReceipt != NULL && strcmp(currentReceipt->date, startDate) == -1) // pronalazi prvi račun koji je veći od startDate
        currentReceipt = currentReceipt->next;
    

    while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {  // zaustavit će se kad nađe račun kasniji od enddatea 
        currentArticle = currentReceipt->headArticle.next;
        while (currentArticle != NULL) {

            if (strcmp(currentArticle->name, article) == 0) {
                *income += currentArticle->quantity * currentArticle->price;
                *count += currentArticle->quantity;
            }
            currentArticle = currentArticle->next;

        }
        currentReceipt = currentReceipt->next;

    }

    return EXIT_SUCCESS;
}

int obrisiArtikle(ArticlePosition articleHead)
{

    ArticlePosition temp = NULL;

    while (articleHead->next != NULL) {
        temp = articleHead->next;
        articleHead->next = temp->next;
        free(temp);
    }

    return 0;
}

int obrisiRacune(ReceiptPosition receiptHead)
{

    ReceiptPosition temp = NULL;

    while (receiptHead->next != NULL) {
        temp = receiptHead->next;
        receiptHead->next = temp->next;
        obrisiArtikle(&temp->headArticle);
        free(temp);
    }

    return 0;
}
