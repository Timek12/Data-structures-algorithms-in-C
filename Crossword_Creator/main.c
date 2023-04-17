#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define SPACE 3

int check_word(const char *word)
{
    int i = 0;
    while(*(word + i))
    {
        if(!isalpha(*(word + i)) || islower(*(word + i))){
            return 1;
        }

        i++;
    }

    return 0;
}
// This function creates a crossword puzzle by taking two words as input
// and creating a matrix of characters with a common letter in them.
// The matrix is stored as a 2D char array and returned through the 'result' parameter.
// The function returns the row index where the common letter is located in the first word.
// If any input parameter is NULL, or if any of the words fail to pass the check_word function,
// the function returns an error code.

int create_leading_word_cross(const char *first, const char *second, char ***result) {
    // Checking for NULL input or empty strings
    if(first == NULL || second == NULL || result == NULL || (int) strlen(first) == 0 || (int) strlen(second) == 0){
    // If result is not NULL, set it to NULL
        if(result != NULL)
            *result = NULL;
    // Return error code -1
        return -1;
    }

    // Checking if either of the words fails the check_word function
    if (check_word(first) || check_word(second)){
        // Set result to NULL and return error code -1
        *result = NULL;
        return -1;
    }

    // Height of the 2D char matrix
    int height = (int) strlen(first);

    // Checking if two words have a common letter
    char common_letter = '-';
    int i, j;
    for(i=0; *(second + i) != '\0'; i++){
        if(strchr(first, *(second + i)) != NULL){
            common_letter = *(second + i);
            break;
        }
    }

    // If no common letter found, set result to NULL and return error code -2
    if(common_letter == '-') {
        *result = NULL;
        return -2;
    }

    // Allocating memory for rows in matrix
    *result = malloc(sizeof(char **) * (height + 1));
    if(*result == NULL){
        return -3;
    }

    // Finding the index of common letter in the first word
    char *p = strchr(first, common_letter);
    long long intersection_row = p - first;

    // Finding the width size of the matrix
    p = strchr(second, common_letter);
    long long width_size = p - second + 1;

    // Looping through each row of the matrix and allocating memory for the characters in the row
    for(i=0; i<height; i++){
        int row_width;
        // If the row contains the common letter, use the second word for the row
        if(i == intersection_row){
            row_width = (int) strlen(second);
            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            if(*(*result + i) == NULL){
                // Freeing memory if allocation fails
                for(j=0; j < i; j++){
                    free(*(*result + j));
                }
                free(*result);
                *result = NULL;
                return -3;
            }
            (*(*(*result + i) + row_width)) = '\0';
            // Copying the second word into the row
            strncpy(*(*result + i), second, strlen(second));
        }
        else{
            // For all other rows, use the width of the intersection row
            row_width = (int) width_size;
            // Allocating memory for the row
            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            // Checking if memory allocation failed
            if(*(*result + i) == NULL){
                for(j=0; j < i; j++){
                    free(*(*result + j));
                }
                free(*result);
                *result = NULL;
                return -3;
            }
            // Adding null character to the end of the row
            (*(*(*result + i) + row_width)) = '\0';
            int k;
            // Filling the row with spaces except for the intersection letter
            for(k = 0; k + 1< row_width; k++){
                strcpy(*(*result + i) + k, " ");
            }
            // Adding the intersection letter to the row
            *(*(*result + i) + k) = *(first + i);
        }
    }
    // Adding null character to the end of the last row to indicate the end of the matrix
    *(*result + height) = NULL;
    // Returning the index of the intersection letter
    return (int) intersection_row;
}


void destroy(char **words)
{
    if(words != NULL){
        for(int i=0; *(words + i) != NULL; i++){
            free(*(words + i));
        }
        free(words);
    }
}

// This function takes four strings: first, second, third and fourth, as well as three pointers to pointers to char arrays: result, first_cross and second_cross.
int create_double_leading_word_cross(
        const char *first, const char *second, const char *third, const char *fourth,
        char ***result, char ***first_cross, char ***second_cross)
{
    // Check if all inputs are non-NULL. Return 1 if any of them are NULL.
    if(first == NULL || second == NULL || third == NULL || fourth == NULL || result == NULL || first_cross == NULL || second_cross == NULL){
        return 1;
    }

    // Create the first cross with the second and first strings.
    int intersection_index_row1 = create_leading_word_cross(second, first, first_cross);

    // Check if there is a problem creating the first cross. Return an error code accordingly.
    switch (intersection_index_row1)
    {
        case -1:
            destroy(*first_cross);
            *result = NULL;
            return 1;

        case -2:
            destroy(*first_cross);
            *result = NULL;
            return 2;

        case -3:
            destroy(*first_cross);
            *result = NULL;
            return 3;

        default:
            break;
    }

    // Create the second cross with the fourth and third strings.
    int intersection_index_row2 = create_leading_word_cross(fourth, third, second_cross);

    // Check if there is a problem creating the second cross. Return an error code accordingly.
    switch (intersection_index_row2)
    {
        case -1:
            destroy(*first_cross);
            destroy(*second_cross);
            *result = NULL;
            return 1;

        case -2:
            destroy(*first_cross);
            destroy(*second_cross);
            *result = NULL;
            return 2;

        case -3:
            destroy(*first_cross);
            destroy(*second_cross);
            *result = NULL;
            return 3;

        default:
            break;
    }

    // Calculate the height of the result and the width of the different parts of the cross.
    int height1 = 0, height2 = 0;
    while(*(*first_cross + height1))
        height1++;

    while(*(*second_cross + height2))
        height2++;

    int smaller_height = (height1 < height2) ? height1 : height2;
    int larger_height = (height1 > height2) ? height1 : height2;

    // Initialize variables for the loop.
    int i, j;

    // Determine the index of the row of the intersection between the two crosses.
    int intersection_index_row = (intersection_index_row1 > intersection_index_row2) ? intersection_index_row1 : intersection_index_row2;

    // Determine the width of the first and second parts of the lower cross and the heights of the upper and lower crosses.
    int first_part_width, second_part_width;
    int lower_cross_intersection_row = (intersection_index_row1 < intersection_index_row2) ? intersection_index_row1 : intersection_index_row2;
    int upper_cross_height = (intersection_index_row1 > intersection_index_row2) ? height1 : height2;
    int lower_cross_height = (intersection_index_row1 < intersection_index_row2) ? height1 : height2;

    // If the intersection row is the same for both crosses, adjust the heights accordingly.
    if(intersection_index_row1 == intersection_index_row2) {
        lower_cross_height = smaller_height;
        upper_cross_height = larger_height;
    }

    int height = larger_height;
    if(height < intersection_index_row + lower_cross_height - lower_cross_intersection_row)
    {
        height = intersection_index_row + lower_cross_height - lower_cross_intersection_row;
    }

    *result = malloc(sizeof (char **) * (height + 1));
    if(*result == NULL){
        destroy(*first_cross);
        destroy(*second_cross);
        return 3;
    }

    int upper_cross_index = 0, lower_cross_index = 0;
    int first_cross_index = 0, second_cross_index = 0;
    for(i=0; i<height; i++){
        int row_width;
        // we consider there four cases:
        // only upper cross
        // intersection row
        // both crosses
        // only lower cross

        // only lower cross
        if(lower_cross_intersection_row + i < intersection_index_row && i != intersection_index_row){

            if(intersection_index_row1 > intersection_index_row2) {
                row_width = (int) strlen(*(*first_cross + upper_cross_index));
            }
            else {
                row_width = (int) strlen(*(*second_cross + upper_cross_index)) + (int) strlen(first) + SPACE;
            }
            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            if (*(*result + i) == NULL) {
                for(j=0; j<i; j++){
                    free(*(*result + j));
                }
                destroy(*first_cross);
                destroy(*second_cross);
                free(*result);
                *result = NULL;
                return 3;
            }

            *(*(*result + i) + row_width) = '\0';

            if(intersection_index_row1 > intersection_index_row2) {
                strncpy(*(*result + i), *(*first_cross + upper_cross_index), strlen(*(*first_cross + upper_cross_index)));
                first_cross_index++;
            }
            else {
                int k;
                for(k = 0; k + (int) strlen(*(*second_cross + upper_cross_index)) < row_width; k++){
                    strcpy(*(*result + i) + k, " ");
                }
                strncpy(*(*result + i) + k, *(*second_cross + upper_cross_index), strlen(*(*second_cross + upper_cross_index)));
                second_cross_index++;
            }
            upper_cross_index++;
        }

        // intersection row
        else if (i == intersection_index_row)
        {
            first_part_width = (int) strlen(third);
            second_part_width = (int) strlen(first);

            row_width = first_part_width  + second_part_width + SPACE;

            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            if (*(*result + i) == NULL) {
                for(j=0; j<i; j++){
                    free(*(*result + j));
                }
                free(*result);
                destroy(*first_cross);
                destroy(*second_cross);
                *result = NULL;
                return 3;
            }

            *(*(*result + i) + row_width) = '\0';

            strncpy(*(*result + i), *(*first_cross + intersection_index_row1), strlen(*(*first_cross + intersection_index_row1)));
            strcpy(*(*result + i) + strlen(*(*first_cross + intersection_index_row1)), "   ");
            strncpy(*(*result + i) + strlen(*(*first_cross + intersection_index_row1)) + SPACE, *(*second_cross + intersection_index_row2), strlen(*(*second_cross + intersection_index_row2)));

            upper_cross_index++;
            lower_cross_index++;
            first_cross_index++;
            second_cross_index++;
        }

        // both crosses row
        else if (upper_cross_index < upper_cross_height && lower_cross_index < lower_cross_height) {

            // CHECK WHICH ONE IS UPPER AND LOWER CROSS

            // first_cross -> upper_cross_index
            // second_cross -> lower_cross_index
            if (height1 == upper_cross_height && intersection_index_row1 > intersection_index_row2) {
                first_part_width = (int) strlen(first);
                second_part_width = (int) strlen(*(*second_cross + lower_cross_index));
                row_width = first_part_width + second_part_width + SPACE;
            } else {
                first_part_width = (int) strlen(first);
                second_part_width = (int) strlen(*(*second_cross + upper_cross_index));
                row_width = first_part_width + second_part_width + SPACE;
            }


            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            if (*(*result + i) == NULL) {
                for (j = 0; j < i; j++) {
                    free(*(*result + j));
                }
                destroy(*first_cross);
                destroy(*second_cross);
                free(*result);
                *result = NULL;
                return 3;
            }
            *(*(*result + i) + row_width) = '\0';

            // first_cross -> upper_cross
            if (height1 == upper_cross_height && intersection_index_row1 > intersection_index_row2) {
                strncpy(*(*result + i), *(*first_cross + upper_cross_index),
                        strlen(*(*first_cross + upper_cross_index)));
                int k;
                for (k = 0; k + (int) strlen(*(*second_cross + lower_cross_index)) +
                            (int) strlen(*(*first_cross + upper_cross_index)) < row_width; k++) {
                    strcpy(*(*result + i) + strlen(*(*first_cross + upper_cross_index)) + k, " ");
                }
                strncpy(*(*result + i) + strlen(*(*first_cross + upper_cross_index)) + k,
                        *(*second_cross + lower_cross_index), strlen(*(*second_cross + lower_cross_index)));
            }
            else {
                strncpy(*(*result + i), *(*first_cross + lower_cross_index),
                        strlen(*(*first_cross + lower_cross_index)));
                int k;
                for (k = 0; k + (int) strlen(*(*second_cross + upper_cross_index)) +
                            (int) strlen(*(*first_cross + lower_cross_index)) < row_width; k++) {
                    strcpy(*(*result + i) + strlen(*(*first_cross + lower_cross_index)) + k, " ");
                }
                strncpy(*(*result + i) + strlen(*(*first_cross + lower_cross_index)) + k,
                        *(*second_cross + upper_cross_index), strlen(*(*second_cross + upper_cross_index)));
            }

            upper_cross_index++;
            lower_cross_index++;
            first_cross_index++;
            second_cross_index++;
        }

        // only lower cross row
        else{


            if(upper_cross_index < upper_cross_height){
                if(height1 == upper_cross_height && first_cross_index < height1) {
                    row_width = (int) strlen(*(*first_cross + upper_cross_index));
                }
                else{
                    row_width = (int) strlen(first) + SPACE + (int) strlen(*(*second_cross + upper_cross_index));
                }
            }
            else{

                if(height1 == lower_cross_height && first_cross_index < height1) {
                    row_width = (int) strlen(*(*first_cross + lower_cross_index));
                }
                else{
                    row_width = (int) strlen(first) + SPACE + (int) strlen(*(*second_cross + lower_cross_index));
                }

            }

            *(*result + i) = malloc(sizeof(char) * (row_width + 1));
            if (*(*result + i) == NULL) {
                for(j=0; j<i; j++){
                    free(*(*result + j));
                }
                free(*result);
                destroy(*first_cross);
                destroy(*second_cross);
                *result = NULL;
                return 3;
            }

            *(*(*result + i) + row_width) = '\0';

            if(upper_cross_index < upper_cross_height){
                if(height1 == upper_cross_height && first_cross_index < height1) {
                    strncpy(*(*result + i), *(*first_cross + upper_cross_index), strlen(*(*first_cross + upper_cross_index)));
                    first_cross_index++;
                }
                else{
                    int k;
                    for(k = 0; k + (int) strlen(*(*second_cross + upper_cross_index)) < row_width; k++){
                        strcpy(*(*result + i) + k, " ");
                    }
                    strncpy(*(*result + i) + k, *(*second_cross + upper_cross_index), strlen(*(*second_cross + upper_cross_index)));
                    second_cross_index++;
                }
                upper_cross_index++;

            }

            else{
                if(height1 == lower_cross_height && first_cross_index < height1) {
                    strncpy(*(*result + i), *(*first_cross + lower_cross_index), strlen(*(*first_cross + lower_cross_index)));
                    first_cross_index++;
                }
                else{
                    int k;
                    for(k = 0; k + (int) strlen(*(*second_cross + lower_cross_index)) < row_width; k++){
                        strcpy(*(*result + i) + k, " ");
                    }
                    strncpy(*(*result + i) + k, *(*second_cross + lower_cross_index), strlen(*(*second_cross + lower_cross_index)));
                    second_cross_index++;
                }
                lower_cross_index++;

            }
        }
    }
    *(*result + height) = NULL;

    return 0;

}


int main() {
    destroy(NULL);
    char *text1 = malloc(sizeof(char) * 11);
    if(text1 == NULL){
        printf("Failed to allocate memory");
        return 8;
    }

    char *text2 = malloc(sizeof(char) * 11);
    if(text2 == NULL){
        printf("Failed to allocate memory");
        free(text1);
        return 8;
    }

    char *text3 = malloc(sizeof(char) * 11);
    if(text3 == NULL){
        printf("Failed to allocate memory");
        free(text1);
        free(text2);
        return 8;
    }

    char *text4 = malloc(sizeof(char) * 11);
    if(text4 == NULL){
        printf("Failed to allocate memory");
        free(text1);
        free(text2);
        free(text3);
        return 8;
    }

    printf("Enter words:");
    if(scanf("%10s %10s %10s %10s", text1, text2, text3, text4) != 4){
        printf("Incorrect input");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
        return 1;
    }
    if(check_word(text1)){
        printf("Incorrect input");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
        return 1;
    }

    if(check_word(text2)){
        printf("Incorrect input");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
        return 1;
    }

    if(check_word(text3)){
        printf("Incorrect input");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
        return 1;
    }

    if(check_word(text4)){
        printf("Incorrect input");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
        return 1;
    }

    char **result;
    char **first_cross;
    char **second_cross;

    int check = create_double_leading_word_cross(text1, text2, text3, text4, &result, &first_cross, &second_cross);
    if(check == 1 || check == 2){
        printf("Unable to make two crosses");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
//        destroy(result);
//        destroy(first_cross);
//        destroy(second_cross);
        return 0;
    }

    if(check == 3){
        printf("Failed to allocate memory");
        free(text1);
        free(text2);
        free(text3);
        free(text4);
//        destroy(result);
//        destroy(first_cross);
//        destroy(second_cross);
        return 8;
    }

    for(int i = 0; *(first_cross + i) != NULL; i++) {
        for(int j = 0; *(*(first_cross + i) + j) != '\0'; j++) {
            printf("%c", *(*(first_cross + i) + j));
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; *(second_cross + i) != NULL; i++) {
        for(int j = 0; *(*(second_cross + i) + j) != '\0'; j++) {
            printf("%c", *(*(second_cross + i) + j));
        }
        printf("\n");
    }
    printf("\n");


    for(int i = 0; *(result + i) != NULL; i++) {
        for(int j = 0; *(*(result + i) + j) != '\0'; j++) {
            printf("%c", *(*(result + i) + j));
        }
        printf("\n");
    }


    free(text1);
    free(text2);
    free(text3);
    free(text4);
    destroy(first_cross);
    destroy(second_cross);
    destroy(result);

    return 0;
}

