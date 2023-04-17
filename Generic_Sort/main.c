#include <stdio.h>
#include <stdlib.h>
#include "comparators.h"

int main() {
// Declare variables for user input.
    int data_type;
    int array_size;

// Prompt user to enter data type (0 for integers, 1 for doubles, 2 for points).
    printf("Enter data type:");

// Check if user input is valid (an integer).
    if (scanf("%d", &data_type) != 1) {
        printf("Incorrect input");
        return 1;
    }

// Check if user input is within valid range (0, 1, or 2).
    if (data_type < 0 || data_type > 2) {
        printf("Incorrect input data");
        return 2;
    }

// Prompt user to enter array size.
    printf("Enter array size:");

// Check if user input is valid (an integer).
    if (scanf("%d", &array_size) != 1) {
        printf("Incorrect input");
        return 1;
    }

// Check if array size is greater than zero.
    if (array_size <= 0) {
        printf("Incorrect input data");
        return 2;
    }

// Declare a void pointer to hold the dynamically allocated array.
    void *tab;

// Use a switch statement to handle the three data types.
    switch (data_type) {
        case 0:
            // Allocate memory for an array of integers.
            tab = malloc(sizeof(int) * array_size);

            // Check if memory allocation was successful.
            if (tab == NULL) {
                printf("Failed to allocate memory");
                return 8;
            }

            // Prompt user to enter integers and populate the array.
            printf("Enter integers:");
            for (int i = 0; i < array_size; i++) {
                int val;

                // Check if user input is valid (an integer).
                if (scanf("%d", &val) != 1) {
                    printf("Incorrect input");

                    // Free the allocated memory and return an error code.
                    free(tab);
                    return 1;
                }
                *((int *) tab + i) = val;
            }

            // Sort the array of integers and print the sorted values.
            sort(tab, array_size, sizeof(int), comp_int);
            for (int i = 0; i < array_size; i++) {
                printf("%d ", *((int *) tab + i));
            }
            break;

        case 1:
            // Allocate memory for an array of doubles.
            tab = malloc(sizeof(double) * array_size);

            // Check if memory allocation was successful.
            if (tab == NULL) {
                printf("Failed to allocate memory");
                return 8;
            }

            // Prompt user to enter doubles and populate the array.
            printf("Enter a doubles:");
            for (int i = 0; i < array_size; i++) {
                double val;

                // Check if user input is valid (a double).
                if (scanf("%lf", &val) != 1) {
                    printf("Incorrect input");

                    // Free the allocated memory and return an error code.
                    free(tab);
                    return 1;
                }
                *((double *) tab + i) = val;
            }
        case 2:
            // Allocate memory for an array of points
            tab = malloc(sizeof(struct point_t) * array_size);

        // Check if memory allocation was successful
            if (tab == NULL) {
                printf("Failed to allocate memory");
                return 8;
            }

        // Prompt user to input coordinates for each point in the array
            printf("Enter x and y coordinates of point:\n");
            for (int i = 0; i < array_size; i++) {
                double x, y;

                // Read in user input and check if it's valid
                if (scanf("%lf %lf", &x, &y) != 2) {
                    printf("Incorrect input");

                    // Free allocated memory and return error code
                    free(tab);
                    return 1;
                }

                // Assign the input coordinates to the current point in the array
                ((struct point_t *) tab + i)->x = x;
                ((struct point_t *) tab + i)->y = y;
            }

            // Sort the array of points using the comp_point function as the comparator
            sort(tab, array_size, sizeof(struct point_t), comp_point);

        // Print out the sorted array of points
            for (int i = 0; i < array_size; i++) {
                printf("%lf %lf\n", ((struct point_t *) tab + i)->x, ((struct point_t *) tab + i)->y);
            }

        default:
            printf("Incorrect input data");
            return 2;


    }
    // Free the allocated memory for the array of points
    free(tab);
    // Return 0 to indicate successful completion of the function
    return 0;
}