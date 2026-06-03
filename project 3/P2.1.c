/***********************************************
*
* @Purpose: Implements a Spam Detector system with functionalities to load training data,
*           display statistics, filter data, evaluate model predictions and allow manual predictions.
* @Author: Ojas Vaidya
* @Creation date: April 13, 2025
* @Date of last modification: April 13, 2025
*
***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100   // Maximum size for strings
#define MAX_EMAILS 100 // Maximum number of emails

// Structure to store prediction results
typedef struct {
    int data_id;    // ID of the email
    char is_spam;  // Spam classification ('y' or 'n')
    float time;    // Time taken for prediction
} Prediction;
// Structure to store email data
typedef struct {
    int email_id;
    char sender_email[MAX_SIZE];
    char category;
    int num_links;
    char spam_classification;
    char training_testing_indicator;
} Email;

/***********************************************
*
* @Purpose: Clears input buffer to remove leftover newline characters after scanf usage.
* @Parameters: ----
* @Return: ----
*
***********************************************/
void clearBuffer() {
    char aux;
    // Clear the input buffer until a newline character is found
    // This is useful to avoid issues with leftover characters in the input buffer
    do {
        scanf("%c", &aux);
    } while (aux != '\n');
}

/***********************************************
*
* @Purpose: Displays the main menu options for user interaction.
* @Parameters: ----
* @Return: ----
*
***********************************************/
void displayMainMenu() {
    // Display the main menu options
    printf("\n");
    printf("1) Load training data\t3) Calculate model metrics\t5) Close\n");
    printf("2) Filter by category\t4) Manual prediction\n");
}

/***********************************************
*
* @Purpose: Loads training data from a file into an array of Email structures.
* @Parameters: 
*     FILE *text_fp: Pointer to the file containing training data.
*     Email emails[]: Array to store loaded email data.
*     int *total_emails: Pointer to store the total number of emails loaded.
* @Return: ----
*
***********************************************/
void loadTrainingData(FILE *text_fp, Email emails[], int *total_emails) {
    int i = 0;            // Initialize loop variable
    char str[MAX_SIZE];   // Temporary string to read lines from the file
    char aux;             // Trash variable to discard unwanted characters
    // Skip first 5 words (e.g., "There is a total of") before reading total emails
    for (i = 0; i < 5; i++) {
        fscanf(text_fp, "%s", str);
    }
    // Read total number of emails
    fscanf(text_fp, "%d", total_emails);
    fgets(str, MAX_SIZE, text_fp); // Read rest of the line to avoid newline issues

    for (i = 0; i < *total_emails; i++) {
        fscanf(text_fp, "%d", &emails[i].email_id);                      // Read email ID (ensures handling of newline issues)
        fscanf(text_fp, "%s", emails[i].sender_email);                   // Read sender email
        fscanf(text_fp, "%s", str);                                      // Skip "Category:"
        fscanf(text_fp, " %c", &emails[i].category);                     // Read actual category (fix: space before %c to skip any newline)
        fscanf(text_fp, "%s %s", str, str);                              // Skip "# links:"
        fscanf(text_fp, "%d", &emails[i].num_links);                     // Read number of links
        fscanf(text_fp, "%s %s", str, str);                              // Skip "Is spam?"
        fscanf(text_fp, " %c", &emails[i].spam_classification);          // Read spam classification
        fscanf(text_fp, " %c", &aux);                                    // Read and discard "[" using `aux`
        fscanf(text_fp, " %c", &emails[i].training_testing_indicator);   // Read training/testing indicator
        fgets(str, MAX_SIZE, text_fp);                                   // Ensure new line is handled correctly
    }
}

/***********************************************
*
* @Purpose: Displays the loaded training data to the user.
* @Parameters: 
*     Email emails[]: Array of emails to display.
*     int total_emails: Total number of emails loaded.
* @Return: ----
*
***********************************************/
void showTrainingData(Email emails[], int total_emails) {
    int i = 0;      // Loop variable
    int count = 0;  // Counter for training data elements
    // Loop through all emails to count training data elements
    for (i = 0; i < total_emails; i++) {
        // Properly format spam indicator
        if (emails[i].training_testing_indicator == '#') {
            count++;
        } 
    }
    // Display the count of training data elements
    
    printf("Training data elements: %d.\n", count);
    for (i = 0; i < total_emails; i++) {
        // Loop through all emails to display training data elements
        if (emails[i].training_testing_indicator == '#') {
            if(emails[i].spam_classification == 'y') { // Properly format spam indicator
                // Display email information with spam classification
                printf("\t! [%c] %d %s: %d links\n",emails[i].category, emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            } 
            else {
                printf("\t  [%c] %d %s: %d links\n", emails[i].category, emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            }
        } 
    }
}

/***********************************************
*
* @Purpose: Converts a lowercase letter to uppercase.
* @Parameters: 
*     char category: The character to convert.
* @Return: char: The uppercase character or the original if already uppercase.
*
***********************************************/
char toUpperCaseLetter(char category) {
    char ch = '\0'; // Initialize character variable
    // Check if the character is lowercase and convert to uppercase
    if (category >= 'a' && category <= 'z') {
        ch =  category - ('a' - 'A'); // Convert to uppercase manually
    }
    return ch; // Return as is if already uppercase
}

/***********************************************
*
* @Purpose: Filters the emails based on the selected category.
* @Parameters: 
*     Email emails[]: Array of emails to filter.
*     int total_emails: Total number of emails loaded.
* @Return: ----
*
***********************************************/
void filterByCategory(Email emails[], int total_emails) {
    char category;          // Variable to store user-selected category
    int valid_input = 0;    // Flag to check if valid input is received
    int i = 0;              // Loop variable
    int found = 0;          // Flag to check if any emails are found in the selected category
    // Loop until a valid category is selected
    while (valid_input == 0) {
        printf("Current categories:\n");
        printf("\t[I]mportant\n");
        printf("\t[P]ersonal\n");
        printf("\t[U]nknown\n");
        printf("Filter by: ");
        scanf(" %c", &category);
        clearBuffer();  // Clear the input buffer
        printf("\n");
        // Convert to uppercase if necessary 
        // Check if the character is lowercase and convert to uppercase
        if (category >= 'a' && category <= 'z') {
            category = category - ('a' - 'A');
        }
        // Check if the category is valid
        // If valid, set valid_input flag to 1
        if (category == 'I' || category == 'P' || category == 'U') {
            valid_input = 1;
        } 
        // If not valid, display error message
        else {
            printf("ERROR: Invalid category.\n\n");
        }
    }
    // Loop through all emails to filter by category
    for (i = 0; i < total_emails; i++) {
        if (emails[i].category == category) {
            found =1;
            // Properly format spam indicator
            // Display email information with spam classification
            if (emails[i].spam_classification == 'y') {
                printf("\t! (%d) %s - %d links ", emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            } else {
                printf("\t  (%d) %s - %d links ", emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            }
            // Check if the email is training or testing data
            if (emails[i].training_testing_indicator == '#') {
                printf("[TRAIN]\n");
            } else {
                printf("[TEST]\n");
            }
        }
    }
    // If no emails are found in the selected category, display a message
    if (!found) {
        printf("No data could be found.\n");
    }
    //printf("\n");
}

/***********************************************
*
* @Purpose: Loads prediction results from a binary file into an array of Prediction structures.
* @Parameters: 
*     char *bin_file: Name of the binary file to read from.
*     Prediction predictions[]: Array to store loaded predictions.
* @Return: int: Total number of predictions loaded or -1 if failed.
*
***********************************************/
int loadPredictions(char *bin_file, Prediction predictions[]) {
    int i = 0;                  // Loop variable
    int total_predictions = 0;  // Total number of predictions loaded
    int read_failed = 0;        // Flag to check if reading failed
    int file_opened = 1;        // Flag to check if file opened successfully
    int result;                 // Variable to store the result of fread
    FILE *bin_fp;               // File pointer for the binary file
    
    // Open the binary file for reading
    bin_fp = fopen(bin_file, "rb");
    // Check if the file opened successfully
    // If not, display an error message
    if (bin_fp == NULL) {
        printf("\nERROR: Could not open file.\n");
        file_opened = 0;
    }
    if (file_opened == 1) {
        // Loop through the maximum number of emails
        for (i = 0; i < MAX_EMAILS; i++) {
            result = fread(&predictions[i], sizeof(Prediction), 1, bin_fp);
            if (result != 1) {
                read_failed = 1;
            }
            // If reading was successful, increment the total predictions count
            if (read_failed == 0) {
                total_predictions = total_predictions + 1;
            }
        }
        fclose(bin_fp);
    }
    if (file_opened == 0) {
        total_predictions = -1;  // Indicate invalid file
    }
    return total_predictions;
}

/***********************************************
*
* @Purpose: Computes evaluation metrics based on email data and predictions.
* @Parameters: 
*     Email emails[]: Array of emails for comparison.
*     int total_emails: Total number of emails loaded.
*     Prediction predictions[]: Array of predictions to evaluate.
*     int total_predictions: Total number of predictions loaded.
* @Return: ----
*
***********************************************/
void computeMetrics(Email emails[], int total_emails, Prediction predictions[], int total_predictions) {
    int i = 0, j = 0;   // Loop variables
    // Initialize variables for metrics
    // True Positive, True Negative, False Positive, False Negative
    int tp = 0, tn = 0, fp_val = 0, fn = 0; 
    // Initialize variables for total time, accuracy, precision, recall, and average time
    float total_time = 0.0, accuracy = 0.0, precision = 0.0, recall = 0.0, avg_time = 0.0; 
    // Initialize variables for processed, found, and valid predictions
    int processed = 0, found = 0, valid_predictions = 0;
    // Loop through all predictions
    while (i < total_predictions) {
        found = 0;
        j = 0;
        // Loop through all emails
        while (j < total_emails) {
            // Check if the email ID matches the prediction data ID
            if (emails[j].email_id == predictions[i].data_id) {
                found = 1;
                processed = 0;
                // Check if the prediction is correct
                if (emails[j].spam_classification == 'y' && predictions[i].is_spam == 'y') { 
                    if (processed == 0) {
                        tp = tp + 1; // True Positive
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification == 'y' && predictions[i].is_spam != 'y') {
                    if (processed == 0) {
                        fn = fn + 1; // False Negative
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification != 'y' && predictions[i].is_spam == 'n') {
                    if (processed == 0) {
                        tn = tn + 1; // True Negative
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification != 'y' && predictions[i].is_spam != 'n') {
                    if (processed == 0) {
                        fp_val = fp_val + 1; // False Positive
                        processed = 1;
                    }
                }
            }
            j = j + 1;
        }
        // If the email ID was found, update valid predictions and total time
        if (found == 1) {
            valid_predictions = valid_predictions + 1;     
            total_time = total_time + predictions[i].time;  
        } 
        else {
            printf("[WARNING] Email %d is not in data file.\n", predictions[i].data_id);
        }

        i = i + 1;
    }
    // Calculate metrics
    // Calculate accuracy
    if ((tp + tn + fp_val + fn) != 0) {
        accuracy = (float)(tp + tn) / (float)(tp + tn + fp_val + fn) * 100.0;
    }
    else {
        accuracy = 0.0;
    }
    // Calculate precision and recall
    if ((tp + fp_val) != 0) {
        precision = (float)tp / (float)(tp + fp_val) * 100.0;
    } else {
        precision = 0.0;
    }
    // Calculate recall
    if ((tp + fn) != 0) {
        recall = (float)tp / (float)(tp + fn) * 100.0;
    } else {
        recall = 0.0;
    }
    // Calculate average prediction time
    if (valid_predictions != 0) {
        avg_time = total_time / valid_predictions; 
    } else {
        avg_time = 0.0;
    }
    // Display metrics
    printf("\nAccuracy: %.2f percent\n", accuracy);
    printf("Precision: %.2f percent\n", precision);
    printf("Recall: %.2f percent\n", recall);
    printf("Average prediction time: %.2fs\n", avg_time);
}

/***********************************************
*
* @Purpose: Prompts user for the binary file containing predictions and computes metrics.
* @Parameters: 
*     Email emails[]: Array of emails for comparison.
*     int total_emails: Total number of emails loaded.
* @Return: ----
*
***********************************************/
void calculateModelMetrics(Email emails[], int total_emails) {
    int total_predictions = 0;          // Total number of predictions loaded
    char bin_file[MAX_SIZE];            // Name of the binary file to read from
    Prediction predictions[MAX_EMAILS]; // Array to store loaded predictions
    
    total_predictions = 0;  
    // Prompt user for the binary file containing predictions
    printf("Where are the prediction results? ");
    scanf("%s", bin_file);
    // Load predictions from the binary file
    total_predictions = loadPredictions(bin_file, predictions);
    // Check if loading was successful
    if (total_predictions >= 0) {
        computeMetrics(emails, total_emails, predictions, total_predictions);
    }
}

/***********************************************
*
* @Purpose: Allows user to make a manual prediction on a specific email.
* @Parameters: 
*     Email emails[]: Array of emails for comparison.
*     int total_emails: Total number of emails loaded.
* @Return: ----
*
***********************************************/
void calculateManualPrediction(Email emails[], int total_emails) {
    int pred_email_id = 0;      // Variable to store user input for email ID
    float pred_time = 0.0;      // Variable to store user input for prediction time
    char spam_choice = '\0';    // Variable to store user input for spam choice
    int valid_id = 0;           // Flag to check if valid email ID is entered
    int valid_time = 0;         // Flag to check if valid time is entered
    int valid_spam = 0;         // Flag to check if valid spam choice is entered
    int i = 0;                  // Loop variable
    int found_index = -1;       // Variable to store the index of the found email
    // Loop until a valid email ID is entered
    while (valid_id == 0) {
        printf("Enter the ID of the email: ");
        scanf("%d", &pred_email_id);
        if (pred_email_id < 0) {
            printf("\nERROR: ID cannot be negative.\n\n");
        } else {
            found_index = -1;
            i = 0;
            // Loop through all emails to find the entered email ID
            while (i < total_emails) {
                if (emails[i].email_id == pred_email_id) {
                    found_index = i;
                }
                i = i + 1;
            }
            if (found_index == -1) {
                printf("\nERROR: ID not found.\n\n");
            } else {
                valid_id = 1;
            }
        }
    }
    // Loop until a valid time is entered
    while (valid_time == 0) {
        printf("Enter the prediction time (in seconds): ");
        scanf("%f", &pred_time);
        clearBuffer(); // Clear the input buffer
        // Check if the entered time is negative
        if (pred_time < 0.0) {
            printf("\nERROR: Time cannot be negative.\n\n");
        } else {
            valid_time = 1;
        }
    }
    // Loop until a valid spam choice is entered
    while (valid_spam == 0) {
        printf("Did the model predict it as spam (y/n)? ");
        scanf("%c", &spam_choice);
        // Convert to uppercase if necessary
        if (spam_choice >= 'a' && spam_choice <= 'z') {
            spam_choice = toUpperCaseLetter(spam_choice);
        }
        if (spam_choice == 'Y' || spam_choice == 'N') {
            valid_spam = 1;
        } else {
            printf("\nERROR: Invalid choice.\n\n");
        }
        clearBuffer();
    }
    // Display the prediction result
    if (emails[found_index].spam_classification == 'y' && spam_choice == 'Y') {
        printf("\nPrediction is correct (TP).\n");
    }
    if (emails[found_index].spam_classification == 'y' && spam_choice != 'Y') {
        printf("\nIncorrect prediction (FN).\n");
    }
    if (emails[found_index].spam_classification != 'y' && spam_choice == 'N') {
        printf("\nPrediction is correct (TN).\n");
    }
    if (emails[found_index].spam_classification != 'y' && spam_choice != 'N') {
        printf("\nIncorrect prediction (FP).\n");
    }
}

/***********************************************
*
* @Purpose: Displays a closing message when the program ends.
* @Parameters: ----
* @Return: ----
*
***********************************************/
void displayEndMessage() {
    printf("Closing Spam Detector...\n");
}

int main() {
    char file[MAX_SIZE];        // Variable to store the file name
    FILE *fp;                   // File pointer for the training data file
    int quit = 0;               // Flag to check if the user wants to quit
    char line[20];              // Variable to store user input for menu options
    char option = '\0';         // Variable to store the selected menu option  
    Email emails[MAX_EMAILS];   // Array to store email data
    int total_emails = 0;       // Total number of emails loaded

    // Get file name from user
    printf("Data filename: ");
    scanf("%s", file);
    clearBuffer();      // Clear the input buffer
    // Open the training data file for reading
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("ERROR: Could not open %s.\n", file);
    } else {
        // File opened successfully
        // Display welcome message
        printf("\nWelcome to Spam Detector!\n");
        // Load training data from the file
        // Load training data into the emails array
        loadTrainingData(fp, emails, &total_emails);
        // Loop until the user chooses to quit
        while (quit == 0) {
            displayMainMenu(); // Display the main menu
            printf("Enter option: ");
            // Read a string (e.g., "hello"), then clear buffer
            scanf("%s", line);
            clearBuffer(); // Clear the input buffer
            printf("\n");
            // If the user typed exactly 1 character, use that as the option
            if (strlen(line) == 1) {
                option = line[0];  
                // Check the selected option and call the corresponding function
                // If the user selected option 1, show training data
                if (option == '1') { 
                    showTrainingData(emails, total_emails);
                } else {
                    // If the user selected option 2, filter by category
                    if (option == '2') {
                        filterByCategory(emails, total_emails);
                    } else {
                        // If the user selected option 3, calculate model metrics
                        if (option == '3') {
                            calculateModelMetrics(emails, total_emails);
                        } else {
                            // If the user selected option 4, allow manual prediction
                            if (option == '4') {
                                calculateManualPrediction(emails, total_emails);
                            } else {
                                // If the user selected option 5, display end message and quit
                                if (option == '5') {
                                    displayEndMessage();
                                    quit = 1;
                                } 
                                // If the user selected an invalid option, display an error message
                                else {
                                    printf("ERROR: Invalid option.\n");
                                }
                            }
                        }
                    }
                }
            } else {
                // The user typed multiple characters (e.g., "hello")
                printf("ERROR: Invalid option.\n");
            }
        }
        fclose(fp);
    }
    return 0;
}