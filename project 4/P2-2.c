/****************************************************************************************
*
* @File: P2-2.c
* @Purpose: Implements the main logic for the LS Ski equipment rental system.
*           Handles file reading, inventory management, pack preparation, and booking.
* @Author: Ojas Vaidya
* @Creation date: 14 May 2025
* @Date of last modification: 15 May 2025
*
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_LINE_LENGTH 100
#define MAX_ITEMS 100

/****************************************************************************************
* @brief Checks if a file can be opened for reading.
* @param filename Name of the file to open.
* @return int 1 if successful, 0 otherwise.
****************************************************************************************/
int readFile(char *filename) {
    int success = 1; //for seeing if the file opened correctly
    FILE *fp;// file pointer
    fp = fopen(filename, "r"); // open the file in read mode

    if (fp == NULL) {// check if the file opened correctly
        printf("ERROR: Could not open %s.\n", filename);
        success = 0;// set success to 0
    } else {
        fclose(fp);// close the file
    }
    return success;// return success
}

/****************************************************************************************
* @brief Parses a date string in DD/MM/YYYY format.
* @param str Date string.
* @param day Pointer to day value.
* @param month Pointer to month value.
* @param year Pointer to year value.
* @return int 1 if successful, 0 otherwise.
****************************************************************************************/
int parseDate(char *str, int *day, int *month, int *year) {
    int success = 1; // check if the date is valid

    *day = (str[0] - '0') * 10 + (str[1] - '0'); //parse the day
    *month = (str[3] - '0') * 10 + (str[4] - '0');//parse the month
    *year = (str[6] - '0') * 1000 + (str[7] - '0') * 100 + (str[8] - '0') * 10 + (str[9] - '0');//parse the year

    if (*day < 1 || *day > 31) {// check if the day is valid
        printf("ERROR: Expected DD/MM/YYYY format for date.\n");
        success = 0;// set success to 0
    }

    if (*month < 1 || *month > 12) {// check if the month is valid
        printf("ERROR: Expected DD/MM/YYYY format for date.\n");
        success = 0;// set success to 0
    }

    return success;// return success
}

/****************************************************************************************
* @brief Validates if a string is in DD/MM/YYYY date format.
* @param str Date string.
* @return int 1 if valid, 0 otherwise.
****************************************************************************************/
int isValidDateFormat(char *str) {
    int i; //loop variable
    int len = 0; // length of the string
    int success = 1;// check if the date is valid
    int loop_validity = 1; // check if the date is valid
    // loop until the end of the string
    while (str[len] != '\0') {
        len++;
    }
    // check if the length of the string is valid
    if (len != 10) {
        success = 0;
    }
    // check if the format is valid
    if (success && (str[2] != '/' || str[5] != '/')) {
        success = 0;
    }
    // check if the format is valid
    if (success) {
        i = 0;
        // loop until the end of the string
        while (i < 10 && loop_validity == 1) {
            // check if the character is valid
            if (i != 2 && i != 5) {
                // check if the character is a digit
                if (str[i] < '0' || str[i] > '9') {
                    success = 0; // set success to 0
                    loop_validity = 0; // set loop_validity to 0
                }
            }
            i++; // increment the loop variable
        }
    }

    return success; // return success
}

/****************************************************************************************
* @brief Checks and parses command-line arguments.
* @param argc Argument count.
* @param argv Argument vector.
* @param filename Pointer to store filename argument.
* @param date Pointer to store date argument.
* @return int 1 if arguments are valid, 0 otherwise.
****************************************************************************************/
int checkArguments(int argc, char *argv[], char **filename, char **date) {
    int valid = 1; // check if the arguments are valid
    // check if the number of arguments is valid
    if (argc != 3) {
        printf("ERROR: Invalid number of arguments.\n");
        valid = 0; // set valid to 0
    }
    // check if the file exists
    if (valid) {
        *filename = argv[1]; // set the filename
        *date = argv[2]; // set the date
        // check if the file exists
        if (!isValidDateFormat(*date)) {
            printf("ERROR: Expected DD/MM/YYYY format for date.\n");
            valid = 0;  // set valid to 0
        }
    }

    return valid; // return valid
}

/****************************************************************************************
* @brief Allows the user to book a pack from available packs.
* @param packs Pointer to the list of packs.
* @param current_date The current date.
* @return void
****************************************************************************************/
void bookPack(LinkedList *packs, Date current_date) {
    int n; // number of packs
    int i, j; // loop variables
    int option; // user option
    Element e; // element to be used
    int check_condition = 1; // check if the user should continue

    n = LINKEDLIST_size(*packs); // get the number of packs
    // check if the number of packs is valid
    if (n == 0) {
        printf("\nThere are no available packs.\n\n");
        check_condition = 0; // set check_condition to 0
    }
    // print the packs
    if (check_condition == 1) {
        printf("\n");
        // loop through the packs
        for (i = 0; i < n; i++) {
            // get the element
            e = LINKEDLIST_get(packs, i);
            // print the pack number, number of items and price
            printf("\t%d. Pack #%d (%d items) for %.2f$:\n", i + 1, i + 1, e.n_items, e.price);
            // loop through the items
            for (j = 0; j < e.n_items; j++) {
                // print the item type, model and brand
                printf("\t\t[%c] %s (%s)\n", e.items[j]->type, e.items[j]->model, e.items[j]->brand);
            }
        }
        // print the back option
        printf("\t%d. Back\n", n + 1);
        // prompt the user for an option
        printf("\tSelect option: ");
        scanf("%d", &option);
        // check if the option is valid
        if (option >= 1 && option <= n) {
            // check if the option is valid
            e = LINKEDLIST_get(packs, option - 1);
            // print the pack number, number of items and price
            for (j = 0; j < e.n_items; j++) {
                e.items[j]->last_rented.day = current_date.day;
                e.items[j]->last_rented.month = current_date.month;
                e.items[j]->last_rented.year = current_date.year + 1;
            }
            // print the item type, model and brand
            LINKEDLIST_remove(packs, option - 1);
            printf("\nPack successfully booked!\n\n");
        } else {
            printf("\n");
        }
    }
}

/****************************************************************************************
* @brief Loads inventory items from a file into a linked list.
* @param list Pointer to the linked list to load items into.
* @param filename Name of the file to load from.
* @return void
****************************************************************************************/
void loadInventory(LinkedList *list, char *filename) {
    
    FILE *fp; // file pointer
    Item *item; // item pointer
    Item **items_array;  // item array pointer
    Element e;  // element pointer
    int total_items;     // total number of items
    char line_content[MAX_LINE_LENGTH]; // line buffer
    int day, month, year;   // date variables
    int read;   // read variable
    int i = 0, j = 0;       // loop variables
    char aux[MAX_LINE_LENGTH];  // auxiliary buffer

    // open the file
    fp = fopen(filename, "r");
    // check if the file opened correctly
    fscanf(fp, "Items in inventory: %d\n", &total_items);
    // check if the number of items is valid
    while (i < total_items) {
        // allocate memory for the item
        item = (Item *) malloc(sizeof(Item));
        // read the item id, type and price
        fscanf(fp, "%d (%c) - %f$\n", &item->id, &item->type, &item->price);
        // allocate memory for the brand
        item->brand = (char *) malloc(MAX_LINE_LENGTH);
        fgets(line_content, MAX_LINE_LENGTH, fp);
        // remove the newline character
        line_content[strlen(line_content) - 1] = '\0';
        strcpy(item->brand, line_content);
        // allocate memory for the model
        item->model = (char *) malloc(MAX_LINE_LENGTH);
        fgets(line_content, MAX_LINE_LENGTH, fp);
        // remove the newline character
        line_content[strlen(line_content) - 1] = '\0';
        strcpy(item->model, line_content);
        // read the number of sizes
        fscanf(fp, "%d:", &item->n_sizes);
        // allocate memory for the sizes
        item->sizes = (float *) malloc(sizeof(float) * item->n_sizes);
        // read the sizes
        for (j = 0; j < item->n_sizes; j++) {
            fscanf(fp, "%f", &item->sizes[j]);
        }
        // read the newline character
        fscanf(fp, "\n"); 
        // read the last rented date
        read = fscanf(fp, "%d/%d/%d\n", &day, &month, &year);
        // check if the date is valid
        if (read == 3) {
            item->last_rented.day = day;
            item->last_rented.month = month;
            item->last_rented.year = year;
        } else {
            // read the date in the wrong format
            fscanf(fp, "%s\n", aux); 
            item->last_rented.day = 0;
            item->last_rented.month = 0;
            item->last_rented.year = 0;
        }
        //allocate memory for the items array
        items_array = (Item **) malloc(sizeof(Item *));
        items_array[0] = item;
        // create the element
        e = LINKEDLIST_createElement(items_array, 1, item->price);
        // allocate memory for the element
        LINKEDLIST_add(list, e, i);
        i++; // increment the loop variable
    }
    // close the file
    fclose(fp);
}

/****************************************************************************************
* @brief Counts the number of items of each type.
* @param type The type of item.
* @param count_s Pointer to count of skis.
* @param count_b Pointer to count of boots.
* @param count_h Pointer to count of helmets.
* @param count_p Pointer to count of poles.
* @return void
****************************************************************************************/
void countItemTypes(char type, int *count_s, int *count_b, int *count_h, int *count_p) {
    if (type == 'S') {
        (*count_s)++;
    } else {
        if (type == 'B') {
            (*count_b)++;
        } else {
            if (type == 'H') {
                (*count_h)++;
            } else {
                if (type == 'P') {
                    (*count_p)++;
                }
            }
        }
    }
}

/****************************************************************************************
* @brief Prints the details of an inventory item.
* @param item Pointer to the item.
* @param current_date The current date.
* @return void
****************************************************************************************/
void printItemDetails(Item *item, Date current_date) {
    int isFuture = 0; // check if the date is in the future
    int j = 0; // loop variable
    // print the item details
    printf("\n[%c] (%d) %s: %s\n", item->type, item->id, item->brand, item->model);
    // print the last rented date
    if (item->last_rented.day > 0 && item->last_rented.month > 0 && item->last_rented.year > 0) {
        // check if the date is valid
        if (item->last_rented.year > current_date.year) {
            isFuture = 1;
        } else {
            // check if the year is the same
            if (item->last_rented.year == current_date.year) {
                // check if the month is valid
                if (item->last_rented.month > current_date.month) {
                    isFuture = 1;
                } else {
                    if (item->last_rented.month == current_date.month) {
                        // check if the day is valid
                        if (item->last_rented.day > current_date.day) {
                            isFuture = 1;
                        }
                    }
                }
            }
        }
        // print the last rented date
        printf("%02d/%02d/%04d", item->last_rented.day, item->last_rented.month, item->last_rented.year);
        // check if the date is in the future
        if (isFuture == 1) {
            printf(" !");
        }
        printf("\n");
    } else {
        printf("Never rented\n");
    }
    // print the price
    printf("Rented for %.2f$\n", item->price);
    // print the sizes
    printf("%d available sizes:\n", item->n_sizes);
    // check if the type is a helmet
    while (j < item->n_sizes) {
        if (item->type == 'H') {
            // print the sizes
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
        j++; // increment the loop variable
    }
}

/****************************************************************************************
* @brief Reviews and displays the inventory with item details and totals.
* @param list The inventory linked list.
* @param current_date The current date.
* @return void
****************************************************************************************/
void reviewInventory(LinkedList list, Date current_date) {

    Element e; // element pointer
    Item *item; // item pointer
    int num_items; // number of items
    int i = 0; // loop variable
    int count_s = 0; // count of skis
    int count_b = 0; // count of boots
    int count_h = 0; // count of helmets
    int count_p = 0; // count of poles
    // get the number of items
    num_items = LINKEDLIST_size(list);
    // while loop to print the items
    while (i < num_items) {
        // get the element
        if (i < LINKEDLIST_size(list)) {
            e = LINKEDLIST_get(&list, i);
            item = e.items[0];
            // print the item details
            printItemDetails(item, current_date);
            // count the item type
            countItemTypes(item->type, &count_s, &count_b, &count_h, &count_p);
        }
        i++; // increment the loop variable
    }
    // print the total number of items
    printf("\nTotal skis: %d\n", count_s);
    printf("Total boots: %d\n", count_b);
    printf("Total helmets: %d\n", count_h);
    printf("Total poles: %d\n\n", count_p);
}

/****************************************************************************************
* @brief Allows the user to prepare a new pack from available inventory items.
* @param inventory The inventory linked list.
* @param packs Pointer to the list of packs.
* @param current_date The current date.
* @return void
****************************************************************************************/
void preparePack(LinkedList inventory, LinkedList *packs, Date current_date) {
    Item *available[MAX_ITEMS]; // array of available items
    Item item; // item pointer
    Item **selected_items; // array of selected items
    Pack new_pack; // pack pointer
    Element e; // element pointer

    int i = 0; // loop variable
    int count = 0; // count of available items
    int total; // total number of items
    int size_pack; // size of the pack
    int id_index; // index of the selected item
    int add_before; // index to add the pack before
    int add_after; // index to add the pack after
    float price; // price of the pack

    total = LINKEDLIST_size(inventory);//
    // Collect available items
    while (i < total && count < MAX_ITEMS) {
        // get the element
        e = LINKEDLIST_get(&inventory, i);
        item = *(e.items[0]); // dereference the item
        // check if the item is available
        // check if the last rented date is valid
        if ((item.last_rented.day == 0 && item.last_rented.month == 0 && item.last_rented.year == 0) ||
            (item.last_rented.year < current_date.year) ||
            (item.last_rented.year == current_date.year && item.last_rented.month < current_date.month) ||
            (item.last_rented.year == current_date.year && item.last_rented.month == current_date.month &&
             item.last_rented.day <= current_date.day)) {
            available[count] = e.items[0];   // store the item
            count++; // increment the count
        }
        i++;// increment the loop variable
    }
    // check if there are available items
    if (count == 0) {
        printf("\nThere are no available items.\n\n");
    } else {
        // print the available items
        printf("\nHow many items will the pack contain? ");
        scanf("%d", &size_pack);
        printf("\n");
        // print the available items
        for (i = 0; i < count; i++) {
            printf("%d) %s (%s), %.2f$ [%c]\n", i + 1, available[i]->model, available[i]->brand, available[i]->price, available[i]->type);
        }
        //allocate memory for the selected items
        selected_items = (Item **)malloc(sizeof(Item *) * size_pack);
        if (selected_items == NULL) {
            printf("ERROR: Failed to prepare pack.\n\n");
        } else {
            // prompt the user for the selected items
            for (i = 0; i < size_pack; i++) {
                // prompt the user for the item index
                printf("Pack item #%d: ", i + 1);
                scanf("%d", &id_index);
                // store the item
                selected_items[i] = available[id_index - 1]; 
            }
            // prompt the user for the price
            printf("Pack price: ");
            scanf("%f", &price);
            
            new_pack.items = selected_items; 
            new_pack.n_items = size_pack;
            new_pack.price = price;
            // create the element
            e = LINKEDLIST_createElement(new_pack.items, new_pack.n_items, new_pack.price);
            //size of the pack
            add_before = LINKEDLIST_size(*packs);
            // add the pack to the list
            LINKEDLIST_add(packs, e, add_before);
            // check if the pack was added successfully
            add_after = LINKEDLIST_size(*packs);
            // check if the pack was added successfully
            if (add_after != add_before + 1) {
                printf("ERROR: Failed to prepare pack.\n\n");
                for (i = 0; i < size_pack; i++) {
                    free(selected_items[i]); // free the item
                }
                free(selected_items); // free the selected items
            } else {
                printf("\nPack added successfully!\n\n");
            }
        }
    }
}
/****************************************************************************************
* @brief Displays the main menu and handles user interaction.
* @param list The inventory linked list.
* @param packs Pointer to the list of packs.
* @param current_date The current date.
* @return void
****************************************************************************************/
void showMainMenu(LinkedList list, LinkedList *packs, Date current_date) {
    int option; // user option
    int keep_run = 1;
    // main menu loop
    // loop until the user selects the exit option
    while (keep_run == 1) {
        // print the menu
        printf("1. Review inventory | 2. Prepare pack | 3. Book pack | 4. Close\n");
        // prompt the user for an option
        printf("Select option: ");
        scanf("%d", &option);
        // switch statement to handle the user option
        switch (option) {
            // review inventory
            case 1:
            reviewInventory(list, current_date);
            break;
            // prepare pack
            case 2:
                preparePack(list, packs, current_date);
                break;
            // book pack
            case 3:
                bookPack(packs, current_date);
                break;
            // exit
            case 4:
                printf("\nHave a nice day!\n");
                keep_run = 0;
                break;
            // invalid option
            default:
                printf("\nERROR: Invalid option.\n\n");
        }
    }
}

/****************************************************************************************
* @brief Program entry point. Handles initial argument checking and program loop.
* @param argc Argument count.
* @param argv Argument vector.
* @return int Exit status.
****************************************************************************************/
int main(int argc, char *argv[]) {
    char *filename; // file name
    char *date; // date
    int day, month, year;   // date variables
    int valid;  // check if the arguments are valid
    LinkedList inventory; // inventory list
    Date current_date;  // current date
    // create the packs list
    LinkedList packs = LINKEDLIST_create();
    // check if the arguments are valid
    valid = checkArguments(argc, argv, &filename, &date);
    // check if the date is valid
    if (valid == 1) {
        valid = parseDate(date, &day, &month, &year);
    }
    // check if the file exists
    if (valid == 1) {
        valid = readFile(filename);
    }
    // create the inventory list
    if (valid == 1) {
        inventory = LINKEDLIST_create();
        // load the inventory
        loadInventory(&inventory, filename);

        current_date.day = day;
        current_date.month = month;
        current_date.year = year;
        // welcome message
        printf("Welcome to LS Ski!\n\n");
        //show the main menu
        showMainMenu(inventory, &packs, current_date);
    }

    return 0;
}