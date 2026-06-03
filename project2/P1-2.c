/*************************************************************
*  @Purpose: LSHealth Manager system that allows managing 
*               doctors and hospital visitations including 
*               adding, listing, removing, and canceling visits.
*  @Author: Ojas Vaidya
*  @Creation date: May 30, 2025
*  @Date of last modification: June 4, 2025
**************************************************************/

#include <stdio.h>
#include <string.h>
/*************************************************************
*  Constant Definitions
*  --------------------
*  MAX_DOCTORS: Maximum number of doctors the system can hold.
*  MAX_CHAR: Maximum number of characters for strings (IDs, names).
*  MAX_HOSPITAL_VISITS: Maximum number of hospital visitations allowed.
**************************************************************/
#define MAX_DOCTORS 30 
#define MAX_CHAR 100
#define MAX_HOSPITAL_VISITS 50

/**
 * @brief Processes visitation details into separate doctor ID, month, and day strings.
 *
 * @param visitation_details The input string in the format <ID>-<month>-<day>.
 * @param doctor_id Output parameter for the extracted doctor ID.
 * @param month Output parameter for the extracted month.
 * @param day Output parameter for the extracted day.
 */
void processVisitDetails(char visitation_details[MAX_CHAR], char doctor_id[MAX_CHAR], char month[MAX_CHAR], char day[MAX_CHAR]) {
    int id_length = 0, start_month = 0, start_day = 0;  // Initialize indices for month and day
    int i = 0, j = 0, count_dash = 0;                   // Count dashes to find positions of month and day
    // Loop through the visitation details to find the positions of dashes
    while (i < strlen(visitation_details)) {
        // Check for the first dash to determine the end of doctor ID
        if (visitation_details[i] == '-' && count_dash == 0) {
            id_length = i;
            start_month = i + 1;
            count_dash++;
        } else {
            // Check for the second dash to determine the start of the day
            if (visitation_details[i] == '-' && count_dash == 1) {
                start_day = i + 1;
                count_dash++;
            }
        }
        i++;
    }
    // Extract the doctor ID from the visitation details
    for (i = 0; i < id_length; i++) {
        doctor_id[i] = visitation_details[i];
    }
    doctor_id[id_length] = '\0';

    // Extract the month from the visitation details
    for (i = start_month; i < start_day - 1; i++) {
        month[j++] = visitation_details[i];
    }
    month[j] = '\0';
    // Reset j for the day extraction
    j = 0;
    // Extract the day from the visitation details
    for (i = start_day; i < strlen(visitation_details); i++) {
        day[j++] = visitation_details[i];
    }
    day[j] = '\0';
}

/**
 * @brief Converts a numeric string to its integer representation.
 *
 * @param str A string representing a numeric value.
 * @return int The integer value converted from the input string.
 */
int myAtoi(char str[]) {
    int value = 0;  // Initialize the integer value to 0
    int i = 0;      // Loop variable to iterate through the string
    // Loop through each character in the string
    for(i = 0; i < strlen(str); i++) {
        value = (value * 10) + (str[i] - '0');
    }
    return value;
}
/**
 * @brief Converts an integer to its string representation.
 *
 * @param number The integer to convert.
 * @param str Output parameter for the string representation of the integer.
 */
void myItoa(int number, char str[]) {
    int i = 0, j = 0;       // Loop variables
    char temp[MAX_CHAR];    // Temporary array to hold the digits of the number
    // Handle the case when number is 0
    while (number > 0) {
        temp[i] = (number % 10) + '0';
        number /= 10;
        i++;
    }
    // If the number is 0, set the first character to '0'
    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    str[j] = '\0';
}
/**
 * @brief Converts a month number to its corresponding name.
 *
 * @param month_num The month number (1-12).
 * @param months_name Output parameter for the name of the month.
 */
void getMonthName(int month_num, char months_name[]) {
    // Switch case to determine the month name based on the month number
    switch (month_num) {
        // case 1 is January, case 2 is February, etc.
        case 1: strcpy(months_name, "January");
                break;
        case 2: strcpy(months_name, "February"); 
                break;
        case 3: strcpy(months_name, "March"); 
                break;
        case 4: strcpy(months_name, "April"); 
                break;
        case 5: strcpy(months_name, "May"); 
                break;
        case 6: strcpy(months_name, "June"); 
                break;
        case 7: strcpy(months_name, "July"); 
                break;
        case 8: strcpy(months_name, "August"); 
                break;
        case 9: strcpy(months_name, "September"); 
                break;
        case 10: strcpy(months_name, "October"); 
                break;
        case 11: strcpy(months_name, "November"); 
                break;
        case 12: strcpy(months_name, "December"); 
                break;
        default: strcpy(months_name, "Invalid"); 
                break;
    }
}
/**
 * @brief Displays a welcome message to the user.
 */
void displayWelcomeMessage() {
    // Print a welcome message to the user
    printf("-----------------------------------------------------------------\n");
    printf("\t\tWELCOME TO LSHEALTH MANAGER\n");
    printf("-----------------------------------------------------------------\n");
}
/**
 * @brief Validates if the input string represents a valid doctor ID (6 digits, not starting with 0).
 *
 * @param id The string representing the doctor ID to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validateDoctorId(char id[]) {
    int valid = 1; // Initialize valid flag to 1 (true)
    int i = 0;     // Loop variable to iterate through the ID string
    // Check if the ID length is exactly 6 and does not start with '0'
    if (strlen(id) != 6 || id[0] == '0') {
        valid = 0;
    } else {
        // Loop through each character in the ID string
        while (i < strlen(id)) {
            if (id[i] < '0' || id[i] > '9') {
                valid = 0;
            }
            i++;
        }
    }
    return valid;
}
/**
 * @brief Validates if the input string represents a valid month (01-12).
 *
 * @param month_char The string representing the month to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validateMonth(char month_char[]) {
    int month = 0; // Initialize month variable to 0
    int check = 0; // Initialize check flag to 0 (false)
    // Convert the month string to an integer
    month = myAtoi(month_char);
    // Check if the month is within the valid range (1-12)
    if (month >= 1 && month <= 12) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}
/**
 * @brief Validates if the input string represents a valid day (01-30).
 *
 * @param day_char The string representing the day to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validateDay(char day_char[]) {
    int day = 0;    // Initialize day variable to 0
    int check = 0;  // Initialize check flag to 0 (false)
    // Convert the day string to an integer
    day = myAtoi(day_char);
    // Check if the day is within the valid range (1-30)
    if (day >= 1 && day <= 30) {
        check = 1;
    } else {
        check = 0;
    }
    return check;
}
/**
 * @brief Validates a patient name to ensure it contains only alphabetic characters and is not empty.
 *
 * @param name The patient name to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validatePatientName(char name[]) {
    int valid = 1;  // Initialize valid flag to 1 (true)
    int i = 0;      // Loop variable to iterate through the name string
    // Check if the name is empty
    if (strlen(name) == 0) {
        valid = 0;
    } else {
        // Loop through each character in the name string
        while (i < strlen(name)) {
            // Check if the character is an alphabetic letter (A-Z or a-z)
            if (!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z'))) {
                valid = 0;
            }
            i++;
        }
    }
    return valid;
}
/**
 * @brief Adds a doctor to the system after validating input and checking for duplicates.
 *
 * @param doctor 2D array storing doctor records.
 * @param count_doctor The current number of doctors.
 * @return int The updated number of doctors.
 */
int addDoctor(char doctor[MAX_DOCTORS][MAX_CHAR], int count_doctor) {
    char input[MAX_CHAR], id[MAX_CHAR], month[MAX_CHAR];   // Input buffer for doctor ID and month
    int count_dash = 0, id_length = 0, start_month = 0;    // Variables to track dashes and positions in the input
    int duplicate = 0, index = 0, match = 0;               // Flags for duplicate checking and matching
    int valid_id = 0, valid_month = 0;                     // Flags for validation of ID and month 
    int i = 0, j = 0, k = 0;                               // Loop variables
    int valid_input = 0;                                   // Flag to check if valid input is received

    // Check if the maximum number of doctors has not been reached
    if (count_doctor < MAX_DOCTORS) {
        // Loop until valid input is received
        while (valid_input == 0) {
            // Prompt user for input
            printf("Enter ID and vacation month (ex. 123456-06): ");
            fgets(input, MAX_CHAR, stdin);
            input[strlen(input) - 1] = '\0'; // Remove the newline character

            // Reset variables
            count_dash = 0;
            id_length = 0;
            start_month = 0;

            // Count the number of dashes in the input
            for (i = 0; i < strlen(input); i++) {
                // If a dash is found, increment the count
                if (input[i] == '-') {
                    count_dash++;
                    // If this is the first dash, set id_length and start_month
                    if (count_dash == 1) {
                        id_length = i;           // Position of the first dash
                        start_month = i + 1;  // Starting index of the month
                    }
                }
            }
            // Check if the input format is correct (exactly one dash)
            if (count_dash != 1) {
                printf("ERROR: Wrong format. Should be <ID>-<month>.\n");
            } else {
                // Extract ID
                // Copy characters from input to id until the first dash
                for (i = 0; i < id_length; i++) {
                    id[i] = input[i];
                }
                id[id_length] = '\0';

                // Extract month
                j = 0;
                // Copy characters from input to month starting after the first dash
                for (i = start_month; i < strlen(input); i++) {
                    month[j] = input[i];
                    j++;
                }
                month[j] = '\0';
                // Check if ID and month are empty
                if (strlen(id) == 0 || strlen(month) == 0) {
                    printf("ERROR: Wrong format. Should be <ID>-<month>.\n");
                } else {
                    // Check if ID is exactly 6 digits
                    if (strlen(id) != 6) {
                        printf("ERROR: ID must be 6 digits.\n");
                    } else {
                        // Check if ID starts with '0'
                        if (id[0] == '0') {
                            printf("ERROR: ID cannot begin with 0.\n");
                        } else {
                            // Check if month is exactly 2 digits
                            if (strlen(month) != 2) {
                                printf("ERROR: Wrong month format. Please use two digits.\n");
                            } else {
                                // Validate the month
                                valid_month = validateMonth(month);
                                // If month is invalid, print error
                                if (valid_month == 0) {
                                    printf("ERROR: Wrong month value. It must be between 01 and 12.\n");
                                } else {
                                    // Validate the doctor ID
                                    valid_id = validateDoctorId(id);
                                    // If ID is valid, check for duplicates
                                    if (valid_id == 1) {
                                        // Check for duplicate
                                        duplicate = 0;
                                        // Loop through existing doctors to find duplicates
                                        for (i = 0; i < count_doctor; i++) {
                                            match = 1;
                                            k = 0;
                                            // Compare the ID with existing doctor IDs
                                            while (k < strlen(id) && match == 1) {
                                                if (doctor[i][k] != id[k]) {
                                                    match = 0;
                                                }
                                                k++;
                                            }
                                            // If a match is found, set duplicate flag
                                            if (match == 1) {
                                                duplicate = 1;
                                            }
                                        }
                                        // If no duplicate is found, add the doctor
                                        if (duplicate == 0) {
                                            // Add the doctor
                                            index = 0;
                                            // Copy ID to doctor array
                                            for (i = 0; i < strlen(id); i++) {
                                                doctor[count_doctor][index] = id[i];
                                                index++;
                                            }
                                            doctor[count_doctor][index] = '-';
                                            index++;
                                            // Copy month to doctor array
                                            for (i = 0; i < strlen(month); i++) {
                                                doctor[count_doctor][index] = month[i];
                                                index++;
                                            }
                                            doctor[count_doctor][index] = '\0';

                                            count_doctor++;
                                            printf("Doctor added successfully.\n");
                                            valid_input = 1;
                                        } else {
                                            printf("Failed to add doctor: Doctor already exists.\n");
                                            valid_input = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        printf("Maximum number of doctors reached.\n");
    }
    return count_doctor;
}
/**
 * @brief Lists all doctors currently in the system, displaying their IDs and vacation months.
 *
 * @param doctor 2D array storing doctor records.
 * @param count_doctor The number of doctors in the system.
 */
void listTotalDoctors(char doctor[MAX_DOCTORS][MAX_CHAR], int count_doctor) {
    int i = 0, j = 0, position_dash = 0, index = 0;   // Loop variables and position tracking
    char id[MAX_CHAR], month[MAX_CHAR];               // Buffers for doctor ID and month
    char months_name[MAX_CHAR];                       // Buffer for month name
    int month_num = 0;                                // Month number for conversion
    // Check if there are any doctors to list
    if (count_doctor > 0) {
        printf("Doctors List (total of %d):\n", count_doctor);
        // Loop through each doctor
        while (i < count_doctor) {
            // Find the position of the dash in the current doctor string
            position_dash = 0;
            // Loop to find the dash position
            while (doctor[i][position_dash] != '-' && doctor[i][position_dash] != '\0') {
                position_dash++;
            }
            // Extract ID
            index = 0;
            // Loop to copy characters from doctor string to id until the dash
            while (index < position_dash) {
                id[index] = doctor[i][index];
                index++;
            }
            id[index] = '\0'; // Null-terminate the ID string
            // Extract month
            index = 0;
            // Start copying characters after the dash
            j = position_dash + 1;
            // Loop to copy characters from doctor string to month until the end
            while (doctor[i][j] != '\0') {
                month[index] = doctor[i][j];
                index++;
                j++;
            }
            month[index] = '\0'; // Null-terminate the month string
            // Convert month to integer
            month_num = myAtoi(month); // Use your `myAtoi` function
            // Determine the month name using helper function
            getMonthName(month_num, months_name);
            // Print the formatted output
            printf("\t%d) Doctor ID: %s, Vacation month: %s\n", i + 1, id, months_name);
            i++;
        }
    } else {
        printf("No doctors exist in the system yet.\n");
    }
}

/**
 * @brief Prompts for and retrieves a valid doctor ID to remove.
 *
 * @param doctor_id Output parameter to store the doctor ID entered by the user.
 * @param count_doctor The number of doctors currently in the system.
 */
void getDoctorID(char doctor_id[MAX_CHAR], int count_doctor){
    // Check if there are any doctors to remove
    if (count_doctor == 0) {
        printf("No doctors exist in the system yet.\n");
    }
    else{
        // Prompt user for doctor ID until a valid one is entered
        printf("Enter the ID of the doctor to remove: ");
        fgets(doctor_id, MAX_CHAR, stdin);
        doctor_id[strlen(doctor_id) - 1] = '\0'; // Remove newline character
        // Validate the doctor ID
        while (strlen(doctor_id) == 0 || strlen(doctor_id) != 6 || doctor_id[0] == '0') {
            printf("ERROR: ID must be 6 digits and cannot begin with 0.\n");
            // Prompt again for a valid doctor ID
            printf("Enter the ID of the doctor to remove: ");
            fgets(doctor_id, MAX_CHAR, stdin);
            doctor_id[strlen(doctor_id) - 1] = '\0'; 
        } 
    }  
}
/**
 * @brief Removes all hospital visits associated with a given doctor ID.
 *
 * @param visits_char 2D array storing hospital visit records.
 * @param count_visitations The current number of visitations.
 * @param doctor_id The doctor ID whose visits are to be removed.
 * @return int The updated number of visitations.
 */
int removeVisitsByDoc(char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR], int count_visitations, char doctor_id[MAX_CHAR]) {
    int v = 0, k = 0, w = 0; // Loop variables
    int match = 0;           // To track if the doctor ID matches

    // Remove visits related to this doctor
    // Loop through all visitations
    for (v = 0; v < count_visitations; v++) {
        match = 1; // Reset match for each visit
        // Check if the doctor ID matches the current visit
        for (k = 0; k < strlen(doctor_id); k++) {
            if (visits_char[v][k] != doctor_id[k]) {
                match = 0;
            }
        }
        // If a match is found and the next character is a dash, remove the visit
        if (match == 1 && visits_char[v][strlen(doctor_id)] == '-') {
            // Shift remaining visits
            // Loop to shift visits left after removal
            for (w = v; w < count_visitations - 1; w++) {
                strcpy(visits_char[w], visits_char[w + 1]);
            }
            count_visitations--; // Decrement visit count
            v--; // Recheck the same index after shifting
        }
    }

    return count_visitations;
}
/**
 * @brief Removes a doctor from the system based on the provided doctor ID.
 *
 * @param doctors 2D array storing doctor records.
 * @param count_doctor The current number of doctors.
 * @param doctor_id The doctor ID to remove.
 * @return int 1 if removal is successful, 0 otherwise.
 */
int removalOfDoctor(char doctors[MAX_DOCTORS][MAX_CHAR], int count_doctor, char doctor_id[MAX_CHAR]) {
    int found = 0;      // Flag to indicate whether the doctor is found
    int i = 0, j = 0;   // Loop variables
    int match = 0;      // To track if the doctor ID matches
    // Check if there are any doctors to remove
    for (i = 0; i < count_doctor && found == 0; i++) {
        match = 1; // Assume match until proven otherwise
        // Loop through each character in the doctor ID
        for (j = 0; j < strlen(doctor_id); j++) {
            // Compare the doctor ID with the current doctor's ID
            if (doctors[i][j] != doctor_id[j]) {
                match = 0; // Mismatch found
            }
        }
        // Check if the match is found and the next character is a dash
         // If a match is found and the next character is a dash, remove the doctor
        if (match == 1 && doctors[i][j] == '-') {
            found = 1; // Doctor found

            // Shift remaining doctors to remove the current one
            // Loop to shift doctors left after removal
            for (j = i; j < count_doctor - 1; j++) {
                strcpy(doctors[j], doctors[j + 1]);
            }
            count_doctor--;
            printf("Doctor removed successfully.\n");
        }
    }

    if (found == 0) {
        printf("Failed to remove doctor: Doctor does not exist.\n");
    }

    return found;
}
/**
 * @brief Adds a hospital visit for a patient after validating doctor ID, month, and day.
 *
 * @param doctor 2D array of doctor records.
 * @param count_doctor The current number of doctors.
 * @param visits_char 2D array storing hospital visit records.
 * @param patient_name 2D array storing patient names for each visit.
 * @param count_visitations The current number of visitations.
 * @return int The updated number of visitations.
 */
int addVisiations(char doctor[MAX_DOCTORS][MAX_CHAR], int count_doctor, char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR], char patient_name[MAX_HOSPITAL_VISITS][MAX_CHAR], int count_visitations) {
    char patient[MAX_CHAR];             // Buffer for patient name
    char visitation_details[MAX_CHAR];  // Buffer for visitation details input
    char doctor_id[MAX_CHAR];           // Buffer for doctor ID
    char month[MAX_CHAR];               // Buffer for month
    char day[MAX_CHAR];                 // Buffer for day
    char vacationMonth[3];              // Buffer for vacation month
    char months_name[MAX_CHAR];         // Buffer for month name
    int found = 0;                      // Flag to indicate if doctor is found  
    int k = 0, l = 0, i = 0;            // Loop variables   
    int match = 0;                      // Flag to check if doctor ID matches
    int valid_patient = 0;              // Flag to check if patient name is valid
    int process_stop = 0;               // Flag to stop the process if needed
    int valid_id = 1;                   // Flag to check if doctor ID is valid
    int valid_month_flag = 1;           // Flag to check if month is valid
    int valid_day_flag = 1;             // Flag to check if day is valid
    int done = 0;                       // Flag to indicate if the process is done
    int count_dash = 0;                 // Counter for dashes in visitation details
    // Check if the maximum number of hospital visits has not been reached
    if (count_visitations >= MAX_HOSPITAL_VISITS) {
        printf("Maximum number of visits reached.\n");
    } else {
        // Check if there are any doctors in the system
        if (count_doctor == 0) {
            printf("No doctors exist in the system yet.\n");
        } else {
            // Ask for patient name once
            printf("Enter your name: ");
            fgets(patient, MAX_CHAR, stdin);
            patient[strlen(patient) - 1] = '\0';
            // Validate patient name
            if (strlen(patient) == 0) {
                printf("ERROR: Name cannot be empty.\n");
            } else {
                // Validate patient name to ensure it contains only letters
                valid_patient = validatePatientName(patient);
                // If patient name is invalid, print error
                if (valid_patient == 0) {
                    printf("ERROR: Name can only contain letters.\n");
                }
            }
            // Only proceed if patient name is valid and not stopped
            if (valid_patient == 1 && process_stop == 0) {
                // Loop until a valid visitation details input is received
                while (!done) {
                    // Prompt user for visitation details
                    printf("Enter doctor ID, month and day of visit (ex. 123456-06-27): ");
                    fgets(visitation_details, MAX_CHAR, stdin);
                    visitation_details[strlen(visitation_details) - 1] = '\0';
                    // Check format: count dashes
                    count_dash = 0;
                    // Count the number of dashes in the visitation details
                    for (i = 0; i < strlen(visitation_details); i++) {
                        // If a dash is found, increment the count
                        if (visitation_details[i] == '-') count_dash++;
                    }
                    // If the number of dashes is not equal to 2, print error
                    if (count_dash != 2) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                        continue;
                    }
                    // Process the visitation details to extract doctor ID, month, and day
                    processVisitDetails(visitation_details, doctor_id, month, day);
                    // Validate the doctor ID
                    valid_id = validateDoctorId(doctor_id);
                    if (!valid_id) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                    }
                    // Validate the month
                    if (strlen(month) != 2 || !validateMonth(month)) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                        valid_month_flag = 0;
                    } else {
                        valid_month_flag = 1;
                    }
                    // Validate the day
                    if (strlen(day) != 2 || !validateDay(day)) {
                        printf("ERROR: Wrong day value. It must be between 01 and 30.\n");
                        valid_day_flag = 0;
                    } else {
                        valid_day_flag = 1;
                    }
                    // If all validations are successful, proceed to check doctor vacation
                    if (valid_id && valid_month_flag && valid_day_flag) {
                        found = 0;
                        // Check if the doctor is on vacation
                        for (k = 0; k < count_doctor; k++) {
                            match = 1;
                            // Compare the doctor ID with the current doctor's ID
                            for (l = 0; l < strlen(doctor_id); l++) {
                                if (doctor[k][l] != doctor_id[l]) {
                                    match = 0;
                                }
                            }
                            // If a match is found and the next character is a dash, check vacation month
                            if (match == 1 && doctor[k][strlen(doctor_id)] == '-') {
                                found = 1;
                                vacationMonth[0] = doctor[k][7];
                                vacationMonth[1] = doctor[k][8];
                                vacationMonth[2] = '\0';
                                // Check if the doctor is on vacation in the specified month
                                if (strcmp(vacationMonth, month) == 0) {
                                    // If the doctor is on vacation, get the month name
                                    getMonthName(myAtoi(month), months_name);
                                    printf("Doctor is on vacation in %s.\n", months_name);
                                    process_stop = 1;
                                }
                            }
                        }
                        // If the doctor is not found or not on vacation, proceed to add the visit
                        if (found == 0 && process_stop == 0) {
                            printf("Doctor %s not found.\n", doctor_id);
                            process_stop = 1;
                        }
                        // If the doctor is found and not on vacation, add the visit
                        if (found == 1 && process_stop == 0) {
                            //copy visitation details to visits_char and patient_name
                            strcpy(visits_char[count_visitations], visitation_details);
                            strcpy(patient_name[count_visitations], patient);
                            count_visitations++;
                            printf("Visit added successfully.\n");
                            done = 1;
                        } else {
                            done = 1;
                        }
                    }
                }
            }
        }
    }
    return count_visitations;
}
/**
 * @brief Lists all visits for a patient by prompting for the patient's name.
 *
 * @param visits_char 2D array storing hospital visit records.
 * @param patient_name 2D array storing patient names for each visit.
 * @param count_visitations The current number of visitations.
 */
void listNumberVisits(char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR], char patient_name[MAX_HOSPITAL_VISITS][MAX_CHAR], int count_visitations) {
    char patient[MAX_CHAR];                                                         // Buffer for patient name
    char doctor_id[MAX_CHAR], day[MAX_CHAR], month[MAX_CHAR], months_name[MAX_CHAR];// Buffers for doctor ID, day, month, and month name
    int valid_input = 0, position_dash1 = 0, position_dash2 = 0, month_num = 0;     // Variables for position tracking and month number
    int found  = 0;                                                                 // Flag to indicate if visits are found         
    int i = 0, k = 0;                                                               // Loop variables           
    int show_visit = 0;                                                             // Counter for showing visits
    // Prompt for patient name at the very beginning
    while (valid_input == 0) {
        // Ask for patient name until a valid one is entered
        printf("Enter your name: ");
        fgets(patient, MAX_CHAR, stdin);
        patient[strlen(patient) - 1] = '\0'; // Remove newline character
        // Validate the patient name
        if (strlen(patient) == 0) {
            printf("ERROR: Name cannot be empty.\n");
        } else {
            // Check if the patient name contains only letters
            if (validatePatientName(patient) == 0) {
                printf("ERROR: Name can only contain letters.\n");
            } else {
                valid_input = 1; // Valid input
            }
        }
    }
    // Search for visits matching the patient name
    for (i = 0; i < count_visitations; i++) {
        // Check if the patient name matches the current visit's patient name
        if (strcmp(patient_name[i], patient) == 0) {
            found++;
        }
    }
    // If no visits are found for the patient, print a message
    if (found == 0) {
        printf("No visits have been found for patient %s.\n", patient);
    } else {
        printf("%d visit(s) found for patient %s:\n", found, patient);
        // Search for visits matching the patient name
        // Loop through all visitations to find matching patient visits
        for (i = 0; i < count_visitations; i++) {
            // If the patient name matches the current visit's patient name
            if (strcmp(patient_name[i], patient) == 0) {
                // Extract doctor ID, month, and day manually
                position_dash1 = 0;
                // Find the position of the first dash
                while (visits_char[i][position_dash1] != '-' && visits_char[i][position_dash1] != '\0') {
                    doctor_id[position_dash1] = visits_char[i][position_dash1];
                    position_dash1++;
                }
                doctor_id[position_dash1] = '\0'; // Null-terminate the doctor ID
                // Find the position of the second dash
                position_dash2 = position_dash1 + 1;
                k = 0;
                // Extract the month from the visit string
                while (visits_char[i][position_dash2] != '-' && visits_char[i][position_dash2] != '\0') {
                    month[k] = visits_char[i][position_dash2];
                    k++;
                    position_dash2++;
                }
                month[k] = '\0'; // Null-terminate the month
                // Extract the day from the visit string
                k = 0;
                position_dash2++;
                // Loop to copy characters from the visit string to day until the end
                while (visits_char[i][position_dash2] != '\0') {
                    day[k] = visits_char[i][position_dash2];
                    k++;
                    position_dash2++;
                }
                day[k] = '\0'; // Null-terminate the day
                // Convert month to integer
                month_num = myAtoi(month);
                // Get month name using helper function
                getMonthName(month_num, months_name);
                // Increment show_visit and print the visit details with correct numbering and integer day
                show_visit++;
                printf("\t%d. Doctor ID: %s --- Date: %d of %s\n", show_visit, doctor_id, myAtoi(day), months_name);
            }
        }
    }
}

/**
 * @brief Cancels a hospital visit for a patient by prompting for the patient's name and visit to cancel.
 *
 * @param visits_char 2D array storing hospital visit records.
 * @param patient_name 2D array storing patient names for each visit.
 * @param count_visitations The current number of visitations.
 * @return int The updated number of visitations.
 */
int cancelVisiations(char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR], char patient_name[MAX_HOSPITAL_VISITS][MAX_CHAR], int count_visitations) {
    char patient[MAX_CHAR];                                                          // Buffer for patient name  
    int index_visitation = 0, valid_input = 0, total_visits = 0;                     // Variables for indexing and counting visits
    int index_search = 0, index_month = 0, index_shift = 0;                          // Loop variables for searching and shifting visits
    int skip_loop = 0;                                                               // Flag to skip the loop if back option is selected                 
    int back = 0;                                                                    // Flag to indicate if back option is selected
    char doctor_id[MAX_CHAR], day[MAX_CHAR], month[MAX_CHAR], months_name[MAX_CHAR]; // Buffers for doctor ID, day, month, and month name
    int position_dash1 = 0, position_dash2 = 0, month_num = 0;                       // Variables for position tracking and month number
    int show_visit = 0;                                                              // Counter for showing visits            
    int cancel_skip = 0;                                                             // Flag to skip cancellation if back option is selected
    int option_visits = 0;                                                           // Variable to store the selected visit option
    char buffer[MAX_CHAR];                                                           // Buffer for user input
    int counter = 1;                                                                 // Counter for visit numbering
    // run a loop to prompt for patient name until a valid one is entered
    while (valid_input == 0) {
        // Prompt for patient name until a valid one is entered
        printf("Enter your name: ");
        fgets(patient, MAX_CHAR, stdin);
        patient[strlen(patient) - 1] = '\0'; // Remove the newline character
        // Validate the patient name
        if (strlen(patient) == 0) {
            printf("ERROR: Name cannot be empty.\n");
        } else {
            // Check if the patient name contains only letters
            if (validatePatientName(patient) == 0) {
                printf("ERROR: Name can only contain letters.\n");
            } else {
                // If the patient name is valid, set valid_input to 1
                valid_input = 1; 
            }
        }
    }

    // Now check if there are any visits for this patient
    total_visits = 0;
    // Loop through all visitations to count visits for the patient
    for (index_visitation = 0; index_visitation < count_visitations; index_visitation++) {
        // If the patient name matches the current visit's patient name
        if (strcmp(patient_name[index_visitation], patient) == 0) {
            total_visits++;
        }
    }
    // If no visits are found for the patient, print a message and set valid_input to -1
    if (count_visitations == 0 || total_visits == 0) {
        printf("No visits have been found for patient %s.\n", patient);
        valid_input = -1; // Special value to indicate exit
    }
    // If visits are found, proceed to list them
    if (valid_input != -1) {
        printf("%d visit(s) found for patient %s:\n", total_visits, patient);
        // Loop through all visitations to list visits for the patient
        for (index_visitation = 0; index_visitation < count_visitations; index_visitation++) {
            // If the patient name matches the current visit's patient name
            if (strcmp(patient_name[index_visitation], patient) == 0) {
                position_dash1 = 0;
                // Find the position of the first dash in the visit string
                while (visits_char[index_visitation][position_dash1] != '-' && visits_char[index_visitation][position_dash1] != '\0') {
                    doctor_id[position_dash1] = visits_char[index_visitation][position_dash1];
                    position_dash1++;
                }
                doctor_id[position_dash1] = '\0';
                // Find the position of the second dash in the visit string
                position_dash2 = position_dash1 + 1;
                index_month = 0;
                // Extract the month from the visit string
                while (visits_char[index_visitation][position_dash2] != '-' && visits_char[index_visitation][position_dash2] != '\0') {
                    month[index_month] = visits_char[index_visitation][position_dash2];
                    index_month++;
                    position_dash2++;
                }
                month[index_month] = '\0';
                // Extract the day from the visit string
                index_month = 0;
                position_dash2++;
                // Loop to copy characters from the visit string to day until the end
                while (visits_char[index_visitation][position_dash2] != '\0') {
                    day[index_month] = visits_char[index_visitation][position_dash2];
                    index_month++;
                    position_dash2++;
                }
                day[index_month] = '\0';
                // Convert month to integer and get month name
                month_num = myAtoi(month);
                // Use the helper function to get the month name
                getMonthName(month_num, months_name);
                // Increment show_visit and print the visit details with correct numbering and integer day
                show_visit++;
                printf("\t%d. Doctor ID: %s --- Date: %d of %s\n", show_visit, doctor_id, myAtoi(day), months_name);
            }
        }
        // Back option
        printf("\t%d. Back\n\n", total_visits + 1);
        // Prompt for the visit to cancel
        valid_input = 0;
        skip_loop = 0;
        // Loop until a valid visit number is entered
        while (valid_input == 0) {
            // Ask for the visit number to cancel
            printf("Enter the number of the visit to cancel: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strlen(buffer) - 1] = '\0'; // Remove newline character
            // Convert the input to an integer
            option_visits = myAtoi(buffer);
            // Check if the input is valid
            if (option_visits >= 1) {
                // Check if the visit number is within the valid range
                if (option_visits <= total_visits) {
                    valid_input = 1;
                } else {
                        // Check if the user selected the back option
                        if (option_visits == total_visits + 1) {
                            //printf("DEBUG: BACK selected (option_visits=%d, total_visits=%d)\n", option_visits, total_visits);
                            cancel_skip = 1;
                            skip_loop = 1;
                            back = 1;
                            valid_input = 1;
                        } else {
                            //printf("DEBUG: Invalid visit number (option_visits=%d, total_visits=%d)\n", option_visits, total_visits);
                            printf("ERROR: Invalid visit number.\n");
                        }
                }
            } else {
                printf("ERROR: Invalid visit number.\n");
            }
            // If the user selected the back option, set skip_loop to 1
            if (skip_loop == 1) {
                valid_input = 1;
            }
        }
        // If the user did not select the back option and the cancel_skip flag is not set
        // proceed to cancel the visit
        if (cancel_skip == 0 && back == 0) {
            // Check if the selected visit number is within the valid range
            if (option_visits <= total_visits) {
                counter = 1;
                // Loop through all visitations to find the visit to cancel
                for (index_search = 0; index_search < count_visitations; index_search++) {
                    // If the patient name matches the current visit's patient name
                    if (strcmp(patient_name[index_search], patient) == 0) {
                        // If the visit number matches the selected visit number
                        if (counter == option_visits) {
                            // loop to shift visits left after cancellation
                            for (index_shift = index_search; index_shift < count_visitations - 1; index_shift++) {
                                // copy the next visit to the current position
                                strcpy(visits_char[index_shift], visits_char[index_shift + 1]);
                                strcpy(patient_name[index_shift], patient_name[index_shift + 1]);
                            }
                            count_visitations--;
                            printf("Visit cancelled successfully.\n");
                            index_search = count_visitations;
                        }
                        // Increment the counter for visit numbering
                        counter++;
                    }
                }
            }
        }
    }
    return count_visitations;
}
/**
 * @brief Main entry point for the LSHealth Manager program.
 *
 * @return int Exit status of the program.
 */
int main(){
    int quit = 0;                                       // Flag to control the main loop
    char command_string[MAX_CHAR];                      // Buffer for user commands  
    int count_doctor = 0;                               // Counter for the number of doctors       
    char doctor[MAX_DOCTORS][MAX_CHAR];                 // 2D array to store doctor records
    char patient_name[MAX_HOSPITAL_VISITS][MAX_CHAR];   // 2D array to store patient names for each visit
    char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR];    // 2D array to store hospital visit records
    int count_visitations = 0;                          // Counter for the number of visitations
    char doctor_id[MAX_CHAR];                           // Buffer for doctor ID   

    // welcome message
    displayWelcomeMessage();
    // use of quit variable to control the main loop
    // The loop continues until the user enters "exit"
    // The user can enter commands to manage doctors and hospital visits
    // The commands include adding/removing doctors, listing doctors, adding/canceling visits, and listing visits
    while(quit == 0){
        // Prompt the user for a command
        printf("admin@LSHealth> ");
        fgets(command_string, MAX_CHAR,stdin);
        command_string[strlen(command_string)-1]='\0';
        // Check the command and call the appropriate function
        // The command can be one of the following:
        // "add doctor", "list doctors", "remove doctor", "add visit", "list visits", "cancel visit", or "exit"
        // If the command is "add doctor", the addDoctor function is called
        if (strcmp(command_string, "add doctor") == 0) {
            count_doctor = addDoctor(doctor, count_doctor);
        }
        else{
            // If the command is "list doctors", the listTotalDoctors function is called
            if (strcmp(command_string, "list doctors") == 0) {
                listTotalDoctors(doctor, count_doctor);
            }
            else{
                // If the command is "remove doctor", the getDoctorID function is called to get the doctor ID
                if(strcmp(command_string, "remove doctor") == 0){
                    // Check if there are any doctors to remove
                    getDoctorID(doctor_id, count_doctor);
                    // If there are doctors, proceed to remove the doctor
                    if(count_doctor != 0) {
                        // Call the removalOfDoctor function to remove the doctor
                        if (removalOfDoctor(doctor, count_doctor, doctor_id)){
                            // If the doctor is removed successfully, update the count
                            count_doctor--;
                            // Remove all visits related to this doctor
                            count_visitations = removeVisitsByDoc(visits_char, count_visitations, doctor_id);
                        }
                    }

                }
                else{
                    // If the command is "add visit", the addVisiations function is called
                    if(strcmp(command_string, "add visit") == 0){
                        count_visitations = addVisiations(doctor, count_doctor, visits_char, patient_name, count_visitations);
                    }
                    else{
                        // If the command is "list visits", the listNumberVisits function is called
                        if(strcmp(command_string, "list visits") == 0){
                            listNumberVisits(visits_char, patient_name, count_visitations);
                        }
                        else{
                            // If the command is "cancel visit", the cancelVisiations function is called
                            if(strcmp(command_string, "cancel visit") == 0){
                                count_visitations =  cancelVisiations(visits_char, patient_name, count_visitations);
                            }
                            else{
                                // If the command is "exit", set quit to 1 to exit the loop
                                if(strcmp(command_string, "exit") == 0){
                                    //printf("Exiting the program. Goodbye!\n");
                                    quit = 1;
                                }
                                else{
                                    // If the command is not recognized, print an error message
                                    printf("ERROR: Command not found.\n");
                                }
                            }
                        }
                    }
                }
            }
        }
    }



    return 0;
}