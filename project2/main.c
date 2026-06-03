#include <stdio.h>
#include <string.h>

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
    int id_length = 0, start_month = 0, start_day = 0; // Initialize indices for month and day
    int i = 0, j = 0, count_dash = 0; // Count dashes to find positions of month and day

    while (i < strlen(visitation_details)) {
        if (visitation_details[i] == '-' && count_dash == 0) {
            id_length = i;
            start_month = i + 1;
            count_dash++;
        } else {
            if (visitation_details[i] == '-' && count_dash == 1) {
                start_day = i + 1;
                count_dash++;
            }
        }
        i++;
    }

    for (i = 0; i < id_length; i++) {
        doctor_id[i] = visitation_details[i];
    }
    doctor_id[id_length] = '\0';

    
    for (i = start_month; i < start_day - 1; i++) {
        month[j++] = visitation_details[i];
    }
    month[j] = '\0';

    j = 0;
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
    int value = 0;
    int i = 0;
    for(i = 0; i < strlen(str); i++) {
        value = (value * 10) + (str[i] - '0');
    }
    return value;
}
/**
 * @brief Converts an integer to its string representation.
 *
 * @param number The integer to convert.
 * @param str The output string buffer to store the result.
 */
void myItoa(int number, char str[]) {
    int i = 0, j = 0;
    char temp[MAX_CHAR];
    while (number > 0) {
        temp[i] = (number % 10) + '0';
        number /= 10;
        i++;
    }
    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    str[j] = '\0';
}

/**
 * @brief Converts a numeric month value to its corresponding month name.
 *
 * @param month_num The numeric value of the month (1-12).
 * @param months_name Output parameter for the month name.
 */
void getMonthName(int month_num, char months_name[]) {
    switch (month_num) {
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
 * @brief Displays the welcome message for the LSHealth Manager.
 */
void displayWelcomeMessage() {
    printf("-----------------------------------------------------------------\n");
    printf("\t\tWELCOME TO LSHEALTH MANAGER\n");
    printf("-----------------------------------------------------------------\n");
}
/**
 * @brief Validates a doctor ID to ensure it is exactly 6 digits and does not start with 0.
 *
 * @param id A string representing the doctor ID to validate.
 * @return int 1 if valid, 0 otherwise.
 */
int validateDoctorId(char id[]) {
    int valid = 1;
    int i = 0;
    if (strlen(id) != 6 || id[0] == '0') {
        valid = 0;
    } else {
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
    int month = 0;
    int check = 0;
    month = myAtoi(month_char);
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
    int day = 0;
    int check = 0;
    day = myAtoi(day_char);
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
    int valid = 1;
    int i = 0;
    if (strlen(name) == 0) {
        valid = 0;
    } else {
        while (i < strlen(name)) {
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
    char input[MAX_CHAR], id[MAX_CHAR], month[MAX_CHAR];
    int count_dash = 0, id_length = 0, start_month = 0;
    int duplicate = 0, index = 0, match = 0;
    int valid_id = 0, valid_month = 0;
    int i = 0, j = 0, k = 0;
    int valid_input = 0;

    if (count_doctor < MAX_DOCTORS) {
        while (valid_input == 0) {
            printf("Enter ID and vacation month (ex. 123456-06): ");
            fgets(input, MAX_CHAR, stdin);
            input[strlen(input) - 1] = '\0'; // Remove the newline character

            // Reset variables
            count_dash = 0;
            id_length = 0;
            start_month = 0;

            // Count the number of dashes in the input
            for (i = 0; i < strlen(input); i++) {
                if (input[i] == '-') {
                    count_dash++;
                    if (count_dash == 1) {
                        id_length = i;           // Position of the first dash
                        start_month = i + 1;  // Starting index of the month
                    }
                }
            }

            if (count_dash != 1) {
                printf("ERROR: Wrong format. Should be <ID>-<month>.\n");
            } else {
                // Extract ID
                for (i = 0; i < id_length; i++) {
                    id[i] = input[i];
                }
                id[id_length] = '\0';

                // Extract month
                j = 0;
                for (i = start_month; i < strlen(input); i++) {
                    month[j] = input[i];
                    j++;
                }
                month[j] = '\0';

                if (strlen(id) == 0 || strlen(month) == 0) {
                    printf("ERROR: Wrong format. Should be <ID>-<month>.\n");
                } else {
                    if (strlen(id) != 6) {
                        printf("ERROR: ID must be 6 digits.\n");
                    } else {
                        if (id[0] == '0') {
                            printf("ERROR: ID cannot begin with 0.\n");
                        } else {
                            if (strlen(month) != 2) {
                                printf("ERROR: Wrong month format. Please use two digits.\n");
                            } else {
                                valid_month = validateMonth(month);
                                if (valid_month == 0) {
                                    printf("ERROR: Wrong month value. It must be between 01 and 12.\n");
                                } else {
                                    // Validate ID and month
                                    valid_id = validateDoctorId(id);

                                    if (valid_id == 1) {
                                        // Check for duplicate
                                        duplicate = 0;
                                        for (i = 0; i < count_doctor; i++) {
                                            match = 1;
                                            k = 0;
                                            while (k < strlen(id) && match == 1) {
                                                if (doctor[i][k] != id[k]) {
                                                    match = 0;
                                                }
                                                k++;
                                            }
                                            if (match == 1) {
                                                duplicate = 1;
                                            }
                                        }

                                        if (duplicate == 0) {
                                            // Add the doctor
                                            index = 0;
                                            for (i = 0; i < strlen(id); i++) {
                                                doctor[count_doctor][index] = id[i];
                                                index++;
                                            }
                                            doctor[count_doctor][index] = '-';
                                            index++;
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
    int i = 0, j = 0, position_dash = 0, index = 0;
    char id[MAX_CHAR], month[MAX_CHAR];
    char months_name[MAX_CHAR];
    int month_num = 0;
    if (count_doctor > 0) {
        printf("Doctors List (total of %d):\n", count_doctor);
        while (i < count_doctor) {
            // Find the position of the dash in the current doctor string
            position_dash = 0;
            while (doctor[i][position_dash] != '-' && doctor[i][position_dash] != '\0') {
                position_dash++;
            }

            // Extract ID
            index = 0;
            while (index < position_dash) {
                id[index] = doctor[i][index];
                index++;
            }
            id[index] = '\0'; // Null-terminate the ID string

            // Extract month
            index = 0;
            j = position_dash + 1;
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
    if (count_doctor == 0) {
        printf("No doctors exist in the system yet.\n");
    }
    else{
        printf("Enter the ID of the doctor to remove: ");
        fgets(doctor_id, MAX_CHAR, stdin);
        doctor_id[strlen(doctor_id) - 1] = '\0'; // Remove newline character

        while (strlen(doctor_id) == 0 || strlen(doctor_id) != 6 || doctor_id[0] == '0') {
            printf("ERROR: ID must be 6 digits and cannot begin with 0.\n");

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
    int v = 0, k = 0, w = 0;
    int match = 0; // To track if the doctor ID matches

    // Remove visits related to this doctor
    for (v = 0; v < count_visitations; v++) {
        match = 1; // Reset match for each visit
        for (k = 0; k < strlen(doctor_id); k++) {
            if (visits_char[v][k] != doctor_id[k]) {
                match = 0;
            }
        }
        if (match == 1 && visits_char[v][strlen(doctor_id)] == '-') {
            // Shift remaining visits
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
    int found = 0; // Flag to indicate whether the doctor is found
    int i = 0, j = 0;
    int match = 0;

    for (i = 0; i < count_doctor && found == 0; i++) {
        match = 1; // Assume match until proven otherwise
        for (j = 0; j < strlen(doctor_id); j++) {
            if (doctors[i][j] != doctor_id[j]) {
                match = 0; // Mismatch found
            }
        }
        if (match == 1 && doctors[i][j] == '-') {
            found = 1; // Doctor found

            // Shift remaining doctors to remove the current one
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
    char patient[MAX_CHAR];
    char visitation_details[MAX_CHAR];
    char doctor_id[MAX_CHAR];
    char month[MAX_CHAR];
    char day[MAX_CHAR];
    char vacationMonth[3];
    char months_name[MAX_CHAR];
    int found = 0;
    int k = 0, l = 0, i = 0;
    int match = 0;
    int valid_patient = 0;
    int process_stop = 0;
    int valid_id = 1;
    int valid_month_flag = 1;
    int valid_day_flag = 1;
    int done = 0;
    int count_dash = 0;

    if (count_visitations >= MAX_HOSPITAL_VISITS) {
        printf("Maximum number of visits reached.\n");
    } else {
        if (count_doctor == 0) {
            printf("No doctors exist in the system yet.\n");
        } else {
            // Ask for patient name once
            printf("Enter your name: ");
            fgets(patient, MAX_CHAR, stdin);
            patient[strlen(patient) - 1] = '\0';
            if (strlen(patient) == 0) {
                printf("ERROR: Name cannot be empty.\n");
            } else {
                valid_patient = validatePatientName(patient);
                if (valid_patient == 0) {
                    printf("ERROR: Name can only contain letters.\n");
                }
            }
            // Only proceed if patient name is valid and not stopped
            if (valid_patient == 1 && process_stop == 0) {
                while (!done) {
                    printf("Enter doctor ID, month and day of visit (ex. 123456-06-27): ");
                    fgets(visitation_details, MAX_CHAR, stdin);
                    visitation_details[strlen(visitation_details) - 1] = '\0';

                    // Check format: count dashes
                    count_dash = 0;
                    //int i;
                    for (i = 0; i < strlen(visitation_details); i++) {
                        if (visitation_details[i] == '-') count_dash++;
                    }
                    if (count_dash != 2) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                        continue;
                    }

                    processVisitDetails(visitation_details, doctor_id, month, day);

                    valid_id = validateDoctorId(doctor_id);
                    if (!valid_id) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                    }

                    if (strlen(month) != 2 || !validateMonth(month)) {
                        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
                        valid_month_flag = 0;
                    } else {
                        valid_month_flag = 1;
                    }

                    if (strlen(day) != 2 || !validateDay(day)) {
                        printf("ERROR: Wrong day value. It must be between 01 and 30.\n");
                        valid_day_flag = 0;
                    } else {
                        valid_day_flag = 1;
                    }

                    if (valid_id && valid_month_flag && valid_day_flag) {
                        found = 0;
                        for (k = 0; k < count_doctor; k++) {
                            match = 1;
                            for (l = 0; l < strlen(doctor_id); l++) {
                                if (doctor[k][l] != doctor_id[l]) {
                                    match = 0;
                                }
                            }

                            if (match == 1 && doctor[k][strlen(doctor_id)] == '-') {
                                found = 1;
                                vacationMonth[0] = doctor[k][7];
                                vacationMonth[1] = doctor[k][8];
                                vacationMonth[2] = '\0';

                                if (strcmp(vacationMonth, month) == 0) {
                                    getMonthName(myAtoi(month), months_name);
                                    printf("Doctor is on vacation in %s.\n", months_name);
                                    process_stop = 1;
                                }
                            }
                        }

                        if (found == 0 && process_stop == 0) {
                            printf("Doctor %s not found.\n", doctor_id);
                            process_stop = 1;
                        }

                        if (found == 1 && process_stop == 0) {
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
    char patient[MAX_CHAR];
    char doctor_id[MAX_CHAR], day[MAX_CHAR], month[MAX_CHAR], months_name[MAX_CHAR];
    int valid_input = 0, position_dash1 = 0, position_dash2 = 0, month_num = 0;
    int found  = 0;
    int i = 0, k = 0;
    int show_visit = 0;
    while (valid_input == 0) {
        printf("Enter your name: ");
        fgets(patient, MAX_CHAR, stdin);
        patient[strlen(patient) - 1] = '\0'; // Remove newline character

        if (strlen(patient) == 0) {
            printf("ERROR: Name cannot be empty.\n");
        } else {
            if (validatePatientName(patient) == 0) {
                printf("ERROR: Name can only contain letters.\n");
            } else {
                valid_input = 1; // Valid input
            }
        }
    }

    // Search for visits matching the patient name
    for (i = 0; i < count_visitations; i++) {
        if (strcmp(patient_name[i], patient) == 0) {
            found++;
        }
    }

    if (found == 0) {
        printf("No visits have been found for patient %s.\n", patient);
    } else {
        printf("%d visit(s) found for patient %s:\n", found, patient);
        
        // Search for visits matching the patient name
        for (i = 0; i < count_visitations; i++) {
            if (strcmp(patient_name[i], patient) == 0) {
                // Extract doctor ID, month, and day manually
                position_dash1 = 0;
                while (visits_char[i][position_dash1] != '-' && visits_char[i][position_dash1] != '\0') {
                    doctor_id[position_dash1] = visits_char[i][position_dash1];
                    position_dash1++;
                }
                doctor_id[position_dash1] = '\0'; // Null-terminate the doctor ID

                position_dash2 = position_dash1 + 1;
                k = 0;
                while (visits_char[i][position_dash2] != '-' && visits_char[i][position_dash2] != '\0') {
                    month[k] = visits_char[i][position_dash2];
                    k++;
                    position_dash2++;
                }
                month[k] = '\0'; // Null-terminate the month

                k = 0;
                position_dash2++;
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
    char patient[MAX_CHAR];
    int index_visitation = 0, valid_input = 0, total_visits = 0;
    int index_search = 0, index_month = 0, index_shift = 0;
    int skip_loop = 0;
    int back = 0;
    char doctor_id[MAX_CHAR], day[MAX_CHAR], month[MAX_CHAR], months_name[MAX_CHAR];
    int position_dash1 = 0, position_dash2 = 0, month_num = 0;
    int show_visit = 0;
    int cancel_skip = 0;
    int option_visits = 0;
    char buffer[MAX_CHAR];
    int counter = 1;
    // Prompt for patient name at the very beginning
    while (valid_input == 0) {
        printf("Enter your name: ");
        fgets(patient, MAX_CHAR, stdin);
        patient[strlen(patient) - 1] = '\0'; // Remove the newline character

        if (strlen(patient) == 0) {
            printf("ERROR: Name cannot be empty.\n");
        } else {
            if (validatePatientName(patient) == 0) {
                printf("ERROR: Name can only contain letters.\n");
            } else {
                valid_input = 1; // Name is valid
            }
        }
    }

    // Now check if there are any visits for this patient
    total_visits = 0;
    for (index_visitation = 0; index_visitation < count_visitations; index_visitation++) {
        if (strcmp(patient_name[index_visitation], patient) == 0) {
            total_visits++;
        }
    }
    if (count_visitations == 0 || total_visits == 0) {
        printf("No visits have been found for patient %s.\n", patient);
        valid_input = -1; // Special value to indicate exit
    }

    if (valid_input != -1) {
        printf("%d visit(s) found for patient %s:\n", total_visits, patient);
        

        for (index_visitation = 0; index_visitation < count_visitations; index_visitation++) {
            if (strcmp(patient_name[index_visitation], patient) == 0) {
                position_dash1 = 0;
                while (visits_char[index_visitation][position_dash1] != '-' && visits_char[index_visitation][position_dash1] != '\0') {
                    doctor_id[position_dash1] = visits_char[index_visitation][position_dash1];
                    position_dash1++;
                }
                doctor_id[position_dash1] = '\0';

                position_dash2 = position_dash1 + 1;
                index_month = 0;
                while (visits_char[index_visitation][position_dash2] != '-' && visits_char[index_visitation][position_dash2] != '\0') {
                    month[index_month] = visits_char[index_visitation][position_dash2];
                    index_month++;
                    position_dash2++;
                }
                month[index_month] = '\0';

                index_month = 0;
                position_dash2++;
                while (visits_char[index_visitation][position_dash2] != '\0') {
                    day[index_month] = visits_char[index_visitation][position_dash2];
                    index_month++;
                    position_dash2++;
                }
                day[index_month] = '\0';

                month_num = myAtoi(month);
                getMonthName(month_num, months_name);

                show_visit++;
                printf("\t%d. Doctor ID: %s --- Date: %d of %s\n", show_visit, doctor_id, myAtoi(day), months_name);
            }
        }
        printf("\t%d. Back\n\n", total_visits + 1);
        
        valid_input = 0;
        skip_loop = 0;
        while (valid_input == 0) {
            printf("Enter the number of the visit to cancel: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strlen(buffer) - 1] = '\0'; // Remove newline character
            option_visits = myAtoi(buffer);
            if (option_visits >= 1) {
                if (option_visits <= total_visits) {
                    valid_input = 1;
                } else {
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
            if (skip_loop == 1) {
                valid_input = 1;
            }
        }
        if (cancel_skip == 0 && back == 0) {
            if (option_visits <= total_visits) {
                counter = 1;
                for (index_search = 0; index_search < count_visitations; index_search++) {
                    if (strcmp(patient_name[index_search], patient) == 0) {
                        if (counter == option_visits) {
                            for (index_shift = index_search; index_shift < count_visitations - 1; index_shift++) {
                                strcpy(visits_char[index_shift], visits_char[index_shift + 1]);
                                strcpy(patient_name[index_shift], patient_name[index_shift + 1]);
                            }
                            count_visitations--;
                            printf("Visit cancelled successfully.\n");
                            index_search = count_visitations;
                        }
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
    int quit = 0;
    char command_string[MAX_CHAR];
    int count_doctor = 0;
    char doctor[MAX_DOCTORS][MAX_CHAR];
    char patient_name[MAX_HOSPITAL_VISITS][MAX_CHAR];
    char visits_char[MAX_HOSPITAL_VISITS][MAX_CHAR];
    int count_visitations = 0;
    char doctor_id[MAX_CHAR];

    displayWelcomeMessage();
    
    while(quit == 0){
        printf("admin@LSHealth> ");
        fgets(command_string, MAX_CHAR,stdin);
        command_string[strlen(command_string)-1]='\0';

        if (strcmp(command_string, "add doctor") == 0) {
            count_doctor = addDoctor(doctor, count_doctor);
        }
        else{
            if (strcmp(command_string, "list doctors") == 0) {
                listTotalDoctors(doctor, count_doctor);
            }
            else{
                if(strcmp(command_string, "remove doctor") == 0){
                    getDoctorID(doctor_id, count_doctor);
                    if(count_doctor != 0) {
                        if (removalOfDoctor(doctor, count_doctor, doctor_id)){
                            count_doctor--;
                            count_visitations = removeVisitsByDoc(visits_char, count_visitations, doctor_id);
                        }
                    }

                }
                else{
                    if(strcmp(command_string, "add visit") == 0){
                        count_visitations = addVisiations(doctor, count_doctor, visits_char, patient_name, count_visitations);
                    }
                    else{
                        if(strcmp(command_string, "list visits") == 0){
                            listNumberVisits(visits_char, patient_name, count_visitations);
                        }
                        else{
                            if(strcmp(command_string, "cancel visit") == 0){
                                count_visitations =  cancelVisiations(visits_char, patient_name, count_visitations);
                            }
                            else{
                                if(strcmp(command_string, "exit") == 0){
                                    //printf("Exiting the program. Goodbye!\n");
                                    quit = 1;
                                }
                                else{
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