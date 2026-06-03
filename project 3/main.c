#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_EMAILS 100

typedef struct {
    int data_id;
    char is_spam;
    float time;
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

void clearBuffer() {
    char aux;
    do {
        scanf("%c", &aux);
    } while (aux != '\n');
}

// Function to display the main menu
void displayMainMenu() {
    printf("\n");
    printf("1) Load training data\t3) Calculate model metrics\t5) Close\n");
    printf("2) Filter by category\t4) Manual prediction\n");
}

void loadTrainingData(FILE *text_fp, Email emails[], int *total_emails) {
    int i = 0;
    char str[MAX_SIZE];
    char aux; // Trash variable to discard unwanted characters
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

void showTrainingData(Email emails[], int total_emails) {
    int i = 0;
    int count = 0;
    for (i = 0; i < total_emails; i++) {
        // Properly format spam indicator
        if (emails[i].training_testing_indicator == '#') {
            count++;
        } 
    }
    printf("Training data elements: %d.\n", count);
    for (i = 0; i < total_emails; i++) {
        // Properly format spam indicator
        if (emails[i].training_testing_indicator == '#') {
            if(emails[i].spam_classification == 'y') {
                printf("\t! [%c] %d %s: %d links\n",emails[i].category, emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            } 
            else {
                printf("\t  [%c] %d %s: %d links\n", emails[i].category, emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            }
        } 
    }
}
// Custom function to convert lowercase to uppercase
char toUpperCaseLetter(char category) {
    char ch = '\0';
    if (category >= 'a' && category <= 'z') {
        ch =  category - ('a' - 'A'); // Convert to uppercase manually
    }
    return ch; // Return as is if already uppercase
}
// Function to filter emails by category
void filterByCategory(Email emails[], int total_emails) {
    char category;
    int valid_input = 0;
    int i = 0;
    int found = 0;

    while (valid_input == 0) {
        printf("Current categories:\n");
        printf("\t[I]mportant\n");
        printf("\t[P]ersonal\n");
        printf("\t[U]nknown\n");
        printf("Filter by: ");
        scanf(" %c", &category);
        clearBuffer();  // Clear the input buffer
        printf("\n");
        /* Convert to uppercase if necessary */
        if (category >= 'a' && category <= 'z') {
            category = category - ('a' - 'A');
        }

        if (category == 'I' || category == 'P' || category == 'U') {
            valid_input = 1;
        } else {
            printf("ERROR: Invalid category.\n\n");
        }
    }

    for (i = 0; i < total_emails; i++) {
        if (emails[i].category == category) {
            found =1;
            if (emails[i].spam_classification == 'y') {
                printf("\t! (%d) %s - %d links ", emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            } else {
                printf("\t  (%d) %s - %d links ", emails[i].email_id, emails[i].sender_email, emails[i].num_links);
            }
            if (emails[i].training_testing_indicator == '#') {
                printf("[TRAIN]\n");
            } else {
                printf("[TEST]\n");
            }
        }
    }
    if (!found) {
        printf("No data could be found.\n");
    }
    //printf("\n");
}

int loadPredictions(char *bin_file, Prediction predictions[]) {
    int i = 0;
    int total_predictions = 0;
    int read_failed = 0;
    int file_opened = 1;
    int result;
    FILE *bin_fp;
    
    bin_fp = fopen(bin_file, "rb");
    if (bin_fp == NULL) {
        printf("\nERROR: Could not open file.\n");
        file_opened = 0;
    }
    if (file_opened == 1) {
        for (i = 0; i < MAX_EMAILS; i++) {
            result = fread(&predictions[i], sizeof(Prediction), 1, bin_fp);
            if (result != 1) {
                read_failed = 1;
            }
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

void computeMetrics(Email emails[], int total_emails, Prediction predictions[], int total_predictions) {
    int i = 0, j = 0;
    int tp = 0, tn = 0, fp_val = 0, fn = 0;
    float total_time = 0.0, accuracy = 0.0, precision = 0.0, recall = 0.0, avg_time = 0.0;
    int processed = 0, found = 0, valid_predictions = 0;
    // Loop through all predictions
    while (i < total_predictions) {
        found = 0;
        j = 0;
        // Loop through all emails
        while (j < total_emails) {
            if (emails[j].email_id == predictions[i].data_id) {
                found = 1;
                processed = 0;
                // Check if the prediction is correct
                if (emails[j].spam_classification == 'y' && predictions[i].is_spam == 'y') { 
                    if (processed == 0) {
                        tp = tp + 1;
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification == 'y' && predictions[i].is_spam != 'y') {
                    if (processed == 0) {
                        fn = fn + 1;
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification != 'y' && predictions[i].is_spam == 'n') {
                    if (processed == 0) {
                        tn = tn + 1;
                        processed = 1;
                    }
                }
                if (emails[j].spam_classification != 'y' && predictions[i].is_spam != 'n') {
                    if (processed == 0) {
                        fp_val = fp_val + 1;
                        processed = 1;
                    }
                }
            }
            j = j + 1;
        }

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

void calculateModelMetrics(Email emails[], int total_emails) {
    int total_predictions = 0;
    char bin_file[MAX_SIZE];
    Prediction predictions[MAX_EMAILS];
    
    total_predictions = 0;
    printf("Where are the prediction results? ");
    scanf("%s", bin_file);
    
    total_predictions = loadPredictions(bin_file, predictions);
    
    if (total_predictions >= 0) {
        computeMetrics(emails, total_emails, predictions, total_predictions);
    }
}

void calculateManualPrediction(Email emails[], int total_emails) {
    int pred_email_id = 0;
    float pred_time = 0.0;
    char spam_choice = '\0';
    int valid_id = 0;
    int valid_time = 0;
    int valid_spam = 0;
    int i = 0;
    int found_index = -1;
    // Loop until a valid email ID is entered
    while (valid_id == 0) {
        printf("Enter the ID of the email: ");
        scanf("%d", &pred_email_id);
        if (pred_email_id < 0) {
            printf("\nERROR: ID cannot be negative.\n\n");
        } else {
            found_index = -1;
            i = 0;
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
        clearBuffer();
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

void displayEndMessage() {
    printf("Closing Spam Detector...\n");
}

int main() {
    char file[MAX_SIZE];
    FILE *fp;
    int quit = 0;
    char line[20];         //  store the user's option input here
    char option = '\0';    //  extract the first character of 'line'
    Email emails[MAX_EMAILS];
    int total_emails = 0;

    // Get file name from user
    printf("Data filename: ");
    scanf("%s", file);
    clearBuffer();

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("ERROR: Could not open %s.\n", file);
    } else {
        printf("\nWelcome to Spam Detector!\n");
        loadTrainingData(fp, emails, &total_emails);

        while (quit == 0) {
            displayMainMenu();
            printf("Enter option: ");
            // Read a string (e.g., "hello"), then clear buffer
            scanf("%s", line);
            clearBuffer();
            printf("\n");
            // If the user typed exactly 1 character, use that as the option
            if (strlen(line) == 1) {
                option = line[0];
                if (option == '1') {
                    showTrainingData(emails, total_emails);
                } else {
                    if (option == '2') {
                        filterByCategory(emails, total_emails);
                    } else {
                        if (option == '3') {
                            calculateModelMetrics(emails, total_emails);
                        } else {
                            if (option == '4') {
                                calculateManualPrediction(emails, total_emails);
                            } else {
                                if (option == '5') {
                                    displayEndMessage();
                                    quit = 1;
                                } else {
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