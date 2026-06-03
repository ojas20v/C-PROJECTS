/***********************************************
*
* @File: main.c
* @Purpose: Implements the main logic for the LS Ski equipment rental system.
*           Handles file reading, inventory management, pack preparation, and booking.
* @Author: Ojas Vaidya
* @Creation date: May 2025
* @Date of last modification: May 2025
*
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_LINE_LENGTH 100
#define MAX_ITEMS 100

 /***********************************************
 *
 * @Purpose: Attempts to open the inventory file to ensure it is accessible.
 * @Parameters: in: filename = path to the inventory file.
 * @Return: 1 if the file was successfully opened; 0 otherwise.
 *
 ************************************************/
int readFile(char *filename) {
    int success = 1;
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("ERROR: Could not open %s.\n", filename);
        success = 0;
    } else {
        fclose(fp);
    }
    return success;
}

 /***********************************************
 *
 * @Purpose: Parses a date string in DD/MM/YYYY format into integers.
 * @Parameters: in: str = input string; out: day, month, year = parsed date components.
 * @Return: 1 if parsing was successful; 0 otherwise.
 *
 ************************************************/
int parseDate(char *str, int *day, int *month, int *year) {
    int success = 1;
    *day = (str[0] - '0') * 10 + (str[1] - '0');
    *month = (str[3] - '0') * 10 + (str[4] - '0');
    *year = (str[6] - '0') * 1000 + (str[7] - '0') * 100 + (str[8] - '0') * 10 + (str[9] - '0');

    if (*day < 1 || *day > 31) {
        printf("ERROR: Expected DD/MM/YYYY format for date.\n");
        success = 0;
    }

    if (*month < 1 || *month > 12) {
        printf("ERROR: Expected DD/MM/YYYY format for date.\n");
        success = 0;
    }

    return success;
}

 /***********************************************
 *
 * @Purpose: Validates whether the input string is in DD/MM/YYYY format.
 * @Parameters: in: str = input date string.
 * @Return: 1 if the format is valid; 0 otherwise.
 *
 ************************************************/
int isValidDateFormat(char *str) {
    int success = 1;
    int len = 0;
    int i;
    int validLoop = 1;

    while (str[len] != '\0') {
        len++;
    }

    if (len != 10) {
        success = 0;
    }

    if (success && (str[2] != '/' || str[5] != '/')) {
        success = 0;
    }

    if (success) {
        i = 0;
        while (i < 10 && validLoop == 1) {
            if (i != 2 && i != 5) {
                if (str[i] < '0' || str[i] > '9') {
                    success = 0;
                    validLoop = 0;
                }
            }
            i++;
        }
    }

    return success;
}

 /***********************************************
 *
 * @Purpose: Validates and extracts command-line arguments.
 * @Parameters: in: argc, argv = argument count and values; out: filename, date = extracted parameters.
 * @Return: 1 if arguments are valid; 0 otherwise.
 *
 ************************************************/
int checkArguments(int argc, char *argv[], char **filename, char **date) {
    int valid = 1;

    if (argc != 3) {
        printf("ERROR: Invalid number of arguments.\n");
        valid = 0;
    }

    if (valid) {
        *filename = argv[1];
        *date = argv[2];

        if (!isValidDateFormat(*date)) {
            printf("ERROR: Expected DD/MM/YYYY format for date.\n");
            valid = 0;
        }
    }

    return valid;
}

void bookPack(LinkedList *packs, Date currentDate) {
    int n = LINKEDLIST_size(*packs);
    int i, j, option;
    Element e;
    int shouldContinue = 1;

    if (n == 0) {
        printf("\nThere are no available packs.\n\n");
        shouldContinue = 0;
    }

    if (shouldContinue == 1) {
        for (i = 0; i < n; i++) {
            e = LINKEDLIST_get(packs, i);

            printf("\n\t%d. Pack #%d (%d items) for %.2f$:\n", i + 1, i + 1, e.n_items, e.price);
            for (j = 0; j < e.n_items; j++) {
                printf("\t\t[%c] %s (%s)\n", e.items[j]->type, e.items[j]->model, e.items[j]->brand);
            }
        }

        printf("\n\t%d. Back\n", n + 1);
        printf("\tSelect option: ");
        scanf("%d", &option);

        if (option >= 1 && option <= n) {
            e = LINKEDLIST_get(packs, option - 1);
            for (j = 0; j < e.n_items; j++) {
                e.items[j]->last_rented.day = currentDate.day;
                e.items[j]->last_rented.month = currentDate.month;
                e.items[j]->last_rented.year = currentDate.year + 1;
            }

            LINKEDLIST_remove(packs, option - 1);
            printf("\nPack successfully booked!\n\n");
        } else {
            printf("\n");
        }
    }
}

void loadInventory(LinkedList *list, char *filename) {
    Item *item;
    FILE *fp;
    int totalItems, i = 0;
    char line[MAX_LINE_LENGTH];
    int d, m, y;
    int read;
    Item **itemArray;
    Element e;
    char dummy[MAX_LINE_LENGTH];
    fp = fopen(filename, "r");
    fscanf(fp, "Items in inventory: %d\n", &totalItems);

    while (i < totalItems) {
        item = (Item *) malloc(sizeof(Item));
        fscanf(fp, "%d (%c) - %f$\n", &item->id, &item->type, &item->price);

        item->brand = (char *) malloc(MAX_LINE_LENGTH);
        fgets(line, MAX_LINE_LENGTH, fp);
        line[strlen(line) - 1] = '\0';
        strcpy(item->brand, line);

        item->model = (char *) malloc(MAX_LINE_LENGTH);
        fgets(line, MAX_LINE_LENGTH, fp);
        line[strlen(line) - 1] = '\0';
        strcpy(item->model, line);

        fscanf(fp, "%d:", &item->n_sizes);
        item->sizes = (float *) malloc(sizeof(float) * item->n_sizes);
        int j = 0;
        for (j = 0; j < item->n_sizes; j++) {
            fscanf(fp, "%f", &item->sizes[j]);
        }
        fscanf(fp, "\n"); // consume the newline using fscanf

        read = fscanf(fp, "%d/%d/%d\n", &d, &m, &y);
        if (read == 3) {
            item->last_rented.day = d;
            item->last_rented.month = m;
            item->last_rented.year = y;
        } else {
            
            fscanf(fp, "%s\n", dummy); // read and discard the line
            item->last_rented.day = 0;
            item->last_rented.month = 0;
            item->last_rented.year = 0;
        }

        itemArray = (Item **) malloc(sizeof(Item *));
        itemArray[0] = item;
        e = LINKEDLIST_createElement(itemArray, 1, item->price);
        LINKEDLIST_add(list, e, i);
        i++;
    }

    fclose(fp);
}

void countItemTypes(char type, int *countS, int *countB, int *countH, int *countP) {
    if (type == 'S') {
        (*countS)++;
    } else {
        if (type == 'B') {
            (*countB)++;
        } else {
            if (type == 'H') {
                (*countH)++;
            } else {
                if (type == 'P') {
                    (*countP)++;
                }
            }
        }
    }
}

void printItemDetails(Item *item, Date currentDate) {
    int isFuture = 0;
    int j = 0;

    printf("\n[%c] (%d) %s: %s\n", item->type, item->id, item->brand, item->model);

    if (item->last_rented.day > 0 && item->last_rented.month > 0 && item->last_rented.year > 0) {
        if (item->last_rented.year > currentDate.year) {
            isFuture = 1;
        } else {
            if (item->last_rented.year == currentDate.year) {
                if (item->last_rented.month > currentDate.month) {
                    isFuture = 1;
                } else {
                    if (item->last_rented.month == currentDate.month) {
                        if (item->last_rented.day > currentDate.day) {
                            isFuture = 1;
                        }
                    }
                }
            }
        }

        printf("%02d/%02d/%04d", item->last_rented.day, item->last_rented.month, item->last_rented.year);
        if (isFuture == 1) {
            printf(" !");
        }
        printf("\n");
    } else {
        printf("Never rented\n");
    }

    printf("Rented for %.2f$\n", item->price);
    printf("%d available sizes:\n", item->n_sizes);

    while (j < item->n_sizes) {
        if (item->type == 'H') {
            switch ((int)item->sizes[j]) {
                case 0: printf("\tXXS\n"); 
                break;
                case 1: printf("\tXS\n"); 
                break;
                case 2: printf("\tS\n"); 
                break;
                case 3: printf("\tM\n"); 
                break;
                case 4: printf("\tL\n"); 
                break;
                case 5: printf("\tXL\n"); 
                break;
                case 6: printf("\tXXL\n"); 
                break;
            }
        } else {
            printf("\t%.2f\n", item->sizes[j]);
        }
        j++;
    }
}

void reviewInventory(LinkedList list, Date currentDate) {
    int i = 0;
    int countS = 0;
    int countB = 0;
    int countH = 0;
    int countP = 0;
    int n = LINKEDLIST_size(list);
    Element e;
    Item *item;

    while (i < n) {
        if (i < LINKEDLIST_size(list)) {
            e = LINKEDLIST_get(&list, i);
        } else {
            break;
        }
        item = e.items[0];

        printItemDetails(item, currentDate);

        // Count all types
        countItemTypes(item->type, &countS, &countB, &countH, &countP);

        i++;
    }

    printf("\nTotal skis: %d\n", countS);
    printf("Total boots: %d\n", countB);
    printf("Total helmets: %d\n", countH);
    printf("Total poles: %d\n\n", countP);
}

void preparePack(LinkedList inventory, LinkedList *packs, Date currentDate) {
    int i = 0, count = 0;
    int total = LINKEDLIST_size(inventory);
    Item *available[MAX_ITEMS];
    Item item;
    Pack newPack;
    float price;
    Element e;
    int packSize;
    Item **selectedItems;

    // Collect available items
    while (i < total && count < MAX_ITEMS) {
        e = LINKEDLIST_get(&inventory, i);
        item = *(e.items[0]);
        if ((item.last_rented.day == 0 && item.last_rented.month == 0 && item.last_rented.year == 0) ||
            (item.last_rented.year < currentDate.year) ||
            (item.last_rented.year == currentDate.year && item.last_rented.month < currentDate.month) ||
            (item.last_rented.year == currentDate.year && item.last_rented.month == currentDate.month &&
             item.last_rented.day <= currentDate.day)) {
            available[count] = e.items[0];
            count++;
        }
        i++;
    }

    if (count == 0) {
        printf("\nThere are no available items.\n\n");
    } else {
        printf("\nHow many items will the pack contain? ");
        scanf("%d", &packSize);
        printf("\n");
        for (i = 0; i < count; i++) {
            printf("%d) %s (%s), %.2f$ [%c]\n", i + 1, available[i]->model, available[i]->brand, available[i]->price, available[i]->type);
            //printf("Last rented: %02d/%02d/%04d\n", available[i]->last_rented.day, available[i]->last_rented.month, available[i]->last_rented.year);

        }

        selectedItems = (Item **)malloc(sizeof(Item *) * packSize);
        if (selectedItems == NULL) {
            printf("ERROR: Failed to prepare pack.\n\n");
        } else {
            for (i = 0; i < packSize; i++) {
                int idx;
                printf("Pack item #%d: ", i + 1);
                scanf("%d", &idx);
                //selectedItems[i] = (Item *)malloc(sizeof(Item));
                //if (selectedItems[i] != NULL) {
                    selectedItems[i] = available[idx - 1];
                //}
            }

            printf("Pack price: ");
            scanf("%f", &price);

            newPack.items = selectedItems;
            newPack.n_items = packSize;
            newPack.price = price;

            e = LINKEDLIST_createElement(newPack.items, newPack.n_items, newPack.price);
            int beforeAdd = LINKEDLIST_size(*packs);
            LINKEDLIST_add(packs, e, beforeAdd);
            int afterAdd = LINKEDLIST_size(*packs);

            if (afterAdd != beforeAdd + 1) {
                printf("ERROR: Failed to prepare pack.\n\n");
                for (i = 0; i < packSize; i++) {
                    free(selectedItems[i]);
                }
                free(selectedItems);
            } else {
                printf("\nPack added successfully!\n\n");
            }
        }
    }
}
 /***********************************************
 *
 * @Purpose: Displays the main menu and handles user option selection.
 * @Parameters: in: list = inventory list; in/out: packs = linked list of packs; in: currentDate = today's date.
 * @Return: ----
 *
 ************************************************/
void showMainMenu(LinkedList list, LinkedList *packs, Date currentDate) {
    int option;
    int keepRunning = 1;

    while (keepRunning == 1) {
        printf("1. Review inventory | 2. Prepare pack | 3. Book pack | 4. Close\n");
        printf("Select option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
            reviewInventory(list, currentDate);
            break;

            case 2:
                //printf("Prepare pack selected.\n");
                preparePack(list, packs, currentDate);
                break;

            case 3:
                //printf("Book pack selected.\n");
                bookPack(packs, currentDate);
                break;

            case 4:
                printf("\nHave a nice day!\n");
                keepRunning = 0;
                break;

            default:
                printf("\nERROR: Invalid option.\n\n");
        }
    }
}

 /***********************************************
 *
 * @Purpose: Main entry point of the program. Validates inputs and starts the rental system.
 * @Parameters: in: argc = number of arguments, argv = list of arguments.
 * @Return: 0 if execution is successful.
 *
 ************************************************/
int main(int argc, char *argv[]) {
    char *filename;
    char *date;
    int day, month, year;
    int valid;
    LinkedList inventory;
    Date currentDate;
    LinkedList packs = LINKEDLIST_create();

    valid = checkArguments(argc, argv, &filename, &date);

    if (valid == 1) {
        valid = parseDate(date, &day, &month, &year);
    }

    if (valid == 1) {
        valid = readFile(filename);
    }

    if (valid == 1) {
        inventory = LINKEDLIST_create();

        loadInventory(&inventory, filename);

        currentDate.day = day;
        currentDate.month = month;
        currentDate.year = year;

        printf("Welcome to LS Ski!\n\n");
        showMainMenu(inventory, &packs, currentDate);
    }

    return 0;
}