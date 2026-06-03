#include <stdio.h>

#define LOW_WEIGHT 18.5
#define NORMAL_WEIGHT 24.9

// Function to display statistics
void displayStatistics(int lowWeightCount, int normalWeightCount, int overweightCount, int totalChildren, int bmiResult) {
    if (bmiResult == 0) {
        printf("Statistics cannot be displayed until BMI has been calculated.\n");
    } 
    else 
    {
        if (totalChildren == 0) {
        printf("No children data to display statistics.\n");
        }
        else {
            printf("Child's statistics:\n");
            printf("Malnourished => %d (%.2f%%)\n", lowWeightCount, (lowWeightCount * 100.0) / totalChildren);
            printf("Normal => %d (%.2f%%)\n", normalWeightCount, (normalWeightCount * 100.0) / totalChildren);
            printf("Overweight => %d (%.2f%%)\n", overweightCount, (overweightCount * 100.0) / totalChildren);
            printf("TOTAL ==== %d\n", totalChildren);
        }
    } 
    
}

// Function to calculate BMI and update statistics
int calculateBMI(int age, float weight, float height) {
    int appointmentWeek;
    int result = 0; // 1: Low weight, 2: Normal weight, 3: Overweight
    float heightInMeters = 0.0;
    float weightInKg = 0.0;
    float bmi = 0.0;
    float weightGainYearly = 0.0, weightGainWeekly = 0.0, weightGainApprox = 0.0, weightGainApproxVisit = 0.0;
    
    heightInMeters = height / 100.0;
    weightInKg = weight / 2.2;
    bmi = weightInKg / (heightInMeters * heightInMeters);

    printf("BMI: %.1f\n", bmi);

    if (bmi < LOW_WEIGHT) {
        printf("This child must follow our nutrition plan!\n");
        result = 1; // Low weight

        do {
            printf("Enter weeks until next appointment: ");
            scanf("%d", &appointmentWeek);
            if (appointmentWeek > 20) {
                printf("ERROR! Must be less than 20 weeks\n");
            }
        } while (appointmentWeek > 20);

        if (age < 7) {
            weightGainYearly = 2.0;
        } 
        else {
            weightGainYearly = 2.4;
        }
        weightGainWeekly = weightGainYearly / 52.0;
        weightGainApprox = appointmentWeek * weightGainWeekly;
        weightGainApproxVisit = weight + (weightGainApprox * 2.2);
        printf("In %d weeks the child should weigh %.2f lb.\n", appointmentWeek, weightGainApproxVisit);

    } 
    else {
        if (bmi <= NORMAL_WEIGHT) {
            printf("This child is at the right weight!\n");
            result = 2; // Normal weight
        } 
        else {
            printf("This child must perform a series of physical exercises!\n");
            result = 3; // Overweight
        }
    }

    return result;
}

// Function to gather child's weight
float childDataWeight() {
    float weight = 0.0;
    do {
        printf("Enter weight (lb): ");
        scanf("%f", &weight);
        if (weight <= 0) {
            printf("The weight must be a positive value!\n");
        }
    } while (weight <= 0);
    return weight;
}

// Function to gather child's height
float childDataHeight() {
    float height = 0.0;
    do {
        printf("Enter height (cm): ");
        scanf("%f", &height);
        if (height <= 0) {
            printf("The height must be a positive value!\n");
        }
    } while (height <= 0);
    return height;
}

// Function to gather child's age
int childDataAge() {
    int age = 0;
    do {
        printf("Child's age: ");
        scanf("%d", &age);
        if (age < 1 || age > 12) {
            printf("The patient is not a child. Enter the age again!\n");
        }
    } while (age < 1 || age > 12);
    return age;
}

// Function to show the menu
void showMenu() {
    printf("\na. Add child data | b. Calculate BMI | c. Statistics | d. Exit\n");
    printf("Select option: ");
}

// Main function
int main() {
    int docID = 0;
    char choice;
    int exitFlag = 0, childData = 0, bmiResult = 0;
    int lowWeightCount = 0, normalWeightCount = 0, overweightCount = 0, totalChildren = 0;
    int age;
    float weight, height;
    int result = 0;

    while (exitFlag == 0) {
        printf("Doctor number: ");
        scanf("%d", &docID);

        if (docID < 100000 || docID > 999999) {
            printf("Invalid doctor number! (must have 6 digits)\n");
            exitFlag = 1;
        } 
        else {
            printf("Welcome to Healthy Child Program! (Dr. id: %d)\n", docID);

            while (exitFlag == 0) {
                showMenu();
                scanf(" %c", &choice);

                switch (choice) {
                    case 'a':
                    case 'A':
                        age = childDataAge();
                        height = childDataHeight();
                        weight = childDataWeight();
                        childData = 1;
                        break;
                    case 'b':
                    case 'B':
                         if (childData) {
                                //check = 1; // Indicate that BMI has been calculated at least once
                                result = calculateBMI(age, weight, height);

                                // Update counters based on the result
                                if (result == 1) {
                                    lowWeightCount++;
                                } 
                                else {
                                    if (result == 2) {
                                        normalWeightCount++;
                                    } 
                                    else {
                                        if (result == 3) {
                                            overweightCount++;
                                        }
                                    }
                                }
                                totalChildren++;
                                bmiResult = 1; // Mark that at least one BMI calculation has occurred
                            } 
                            else {
                                printf("BMI cannot be calculated until data is inserted.\n");
                            }
                            break;

                        case 'c':
                        case 'C':
                            if (bmiResult) { // Ensure BMI was calculated at least once
                                displayStatistics(lowWeightCount, normalWeightCount, overweightCount, totalChildren, bmiResult);
                            } else {
                                printf("Statistics cannot be displayed until BMI has been calculated.\n");
                            }
                            break;
                    case 'd':
                    case 'D':
                        printf("See you in the next appointment!!\n");
                        exitFlag = 1;
                        break;
                    default:
                        printf("Wrong option!\n");
                        break;
                }
            }
        }
    }
    return 0;
}
