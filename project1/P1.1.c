/*******************************************************************
*
* @Purpose: 
* @Author: OJAS VAIDYA
* @Creation date: [24/01/202]
* @Date of last modification: [25/01/2025]
*
*********************************************************************/
 #include <stdio.h>

#define LOW_WEIGHT 18.5
#define NORMAL_WEIGHT 24.9
/*************************************************************************************
 *
 * @Purpose: To display the statistics of child BMI classifications in the system.
 * @Parameters:
 *     in: lowWeightCount - Number of children classified as malnourished.
 *     in: normalWeightCount - Number of children classified with normal weight.
 *     in: overweightCount - Number of children classified as overweight.
 *     in: totalChildren - Total number of children recorded in the system.
 *     in: bmiResult - Flag indicating whether BMI has been calculated (1: Yes, 0: No).
 * @Return: ----
 *
 *************************************************************************************/
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
/**********************************************************************
 *
 * @Purpose: Calculates BMI for a child and provides recommendations.
 * @Parameters:
 *     in: age - Age of the child.
 *     in: weight - Weight of the child in pounds.
 *     in: height - Height of the child in centimeters.
 * @Return: int - BMI classification (1: Low weight, 2: Normal weight, 3: Overweight).
 *
 **********************************************************************/
int calculateBMI(int age, float weight, float height) {
    // Variables for BMI calculation
    int appointmentWeek;                 // Weeks until the next appointment
    int result = 0;                      // 1: Low weight, 2: Normal weight, 3: Overweight
    float heightInMeters = 0.0;          // Height in meters
    float weightInKg = 0.0;              // Weight in kilograms
    float bmi = 0.0;                     // BMI value
    float weightGainYearly = 0.0;        // Estimated yearly weight gain
    float weightGainWeekly = 0.0;        // Estimated weekly weight gain
    float weightGainApprox = 0.0;        // Approximate weight gain until next visit
    float weightGainApproxVisit = 0.0;   // Approximate weight after next visit
    
    heightInMeters = height / 100.0; // Conversion of Height into meters
    weightInKg = weight / 2.2; // Conversion of Weight into kilograms
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
        // Determine yearly weight gain based on age
        weightGainWeekly = weightGainYearly / 52.0;                 // Convert yearly gain to weekly
        weightGainApprox = appointmentWeek * weightGainWeekly;      // Gain until next visit
        weightGainApproxVisit = weight + (weightGainApprox * 2.2);  // Convert back to pounds
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
/*****************************************************************************
 *
 * @Purpose: Collects and validates the weight of a child.
 * @Parameters: ----
 * @Return: float - Validated weight of the child.
 *
 *****************************************************************************/
float childDataWeight() {
    float weight = 0.0;  // Child's weight in pounds
    do {
        printf("Enter weight (lb): ");
        scanf("%f", &weight);
        if (weight <= 0) {
            printf("The weight must be a positive value!\n");
        }
    } while (weight <= 0);
    return weight;
}
/*****************************************************************************
 *
 * @Purpose: Collects and validates the height of a child.
 * @Parameters: ----
 * @Return: float - Validated height of the child.
 *
 *****************************************************************************/
float childDataHeight() {
    float height = 0.0;  // Child's height in centimeters
    do {
        printf("Enter height (cm): ");
        scanf("%f", &height);
        if (height <= 0) {
            printf("The height must be a positive value!\n");
        }
    } while (height <= 0);
    return height;
}
/*****************************************************************************
 *
 * @Purpose: Collects and validates the age of a child.
 * @Parameters: ----
 * @Return: int - Validated age of the child.
 *
 *****************************************************************************/
int childDataAge() {
    int age = 0;  // Child's age in years
    do {
        printf("Child's age: ");
        scanf("%d", &age);
        if (age < 1 || age > 12) {
            printf("The patient is not a child. Enter the age again!\n");
        }
    } while (age < 1 || age > 12);
    return age;
}
/**********************************************************
 *
 * @Purpose: Displays the program's main menu options.
 * @Parameters: ----
 * @Return: ----
 *
 **********************************************************/
// Function to show the menu
void showMenu() {
    printf("\na. Add child data | b. Calculate BMI | c. Statistics | d. Exit\n");
    printf("Select option: ");
}
/***********************************************
 *
 * @Purpose: Main entry point for the program. It handles:
 * 1. Doctor ID validation.
 * 2. Displaying the menu and executing the selected functionality.
 * 3. Exit management for the program.
 * @Parameters: ----
 * @Return: int - Returns 0 upon successful execution.
 *
 ***********************************************/
int main() {
    int docID = 0;               // Doctor ID number
    char choice;                 // User's menu choice
    int exitFlag = 0;            // Exit flag for the program loop
    int childData = 0;           // Flag indicating if child data has been entered
    int bmiResult = 0;           // Flag indicating if BMI has been calculated
    int lowWeightCount = 0;      // Count of malnourished children
    int normalWeightCount = 0;   // Count of children with normal weight
    int overweightCount = 0;     // Count of overweight children
    int totalChildren = 0;       // Total number of children
    int age;                     // Child's age
    float weight, height;        // Child's weight and height
    int result = 0;              // Result from BMI calculation

    while (exitFlag == 0) {
        printf("Doctor number: ");
        scanf("%d", &docID);

        if (docID < 100000 || docID > 999999) {
            printf("Invalid doctor number! (must have 6 digits)\n");
            exitFlag = 1;
        } else {
            printf("Welcome to Healthy Child Program! (Dr. id: %d)\n", docID);

            while (exitFlag == 0) {
                showMenu();
                scanf(" %c", &choice);

                switch (choice) {
                    case 'a':
                    case 'A':
                        age = childDataAge();        // to get the age of the child
                        height = childDataHeight();  // to get the height of the child
                        weight = childDataWeight();  // to get the weight of the child 
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
