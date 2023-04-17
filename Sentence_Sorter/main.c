#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// function to count sentences in the text
int count_sentences(const char *text) {
    int i = 0;
    int number_of_sentences = 0;
    while (*(text + i)) {
        if (*(text + i) == '.') {
            number_of_sentences++;
        }
        i++;
    }
    return number_of_sentences;
}

// function to free allocated memory for the text 3d array
void destroy(char ***words) {
    if (words != NULL && *words != NULL) {
        for(int i=0; *(words + i) != NULL; i++){
            for(int j=0; *(*(words + i) + j) != NULL; j++){
                free(*(*(words + i) + j));
                *(*(words + i) + j) = NULL;
            }
            free(*(words + i));
            *(words + i) = NULL;
        }
    }

    if(words != NULL)
        free(words);

}

// function to split text into sentences, words and characters
int split_sentences(const char *text, char ****output) {
    // check if input is null
    if (text == NULL) {
        *output = NULL;
        return 1;
    }

    // count the number of sentences in the input text
    int number_of_sentences = count_sentences(text);

    // if there are no sentences in the input text, return an error
    if (number_of_sentences == 0) {
        *output = NULL;
        return 2;
    }

    // allocate memory for the output array
    *output = malloc(sizeof(char **) * (number_of_sentences + 1));

    // if memory allocation fails, free the previously allocated memory and return an error
    if (*output == NULL) {
        free(*output);
        *output = NULL;
        return 3;
    }

    // initialize counters and flags
    int i = 0, j = 0, k;
    int word_count = 0;
    int is_char = 0;

    // loop through each character in the input text
    while (*(text + i)) {
        // check if character is a letter
        if (isalpha(*(text + i))) {
            is_char = 1;
        }
            // check if character is a period (end of sentence)
        else if (*(text + i) == '.') {
            // if previous character was a letter, increment the word count
            if (is_char) {
                word_count++;
            }

            // allocate memory for a new sentence in the output array
            *(*output + j) = malloc(sizeof(char *) * (word_count + 1));

            // if memory allocation fails, free the previously allocated memory and return an error
            if (*(*output + j) == NULL) {
                destroy(*output);
                *output = NULL;
                return 3;
            }

            // reset counters and flags for the next sentence
            word_count = 0;
            is_char = 0;
            j++;
        }
            // check if character is a space or punctuation mark (part of a word)
        else {
            // if previous character was a letter, increment the word count and reset the is_char flag
            if (is_char) {
                word_count++;
                is_char = 0;
            }
        }
        // move to the next character in the input text
        i++;
    }

    // set the last element of the output array to null
    *(*output + j) = NULL;

    i = 0,       j = 0,      k = 0;
    // chars     words     sentences
    // loop through each character in the input text again
    while (*(text + i)) {
        // check if character is a period (end of sentence)
        if (*(text + i) == '.') {
            // set the last element of the current sentence to null
            *(*(*output + k) + j) = NULL;

            // move to the next sentence
            k++;
            j = 0;
            i++;
        }
            // check if character is a letter (part of a word)
        else if (isalpha(*(text + i))) {
            // initialize word length counter
            int word_len = 0;

            // loop through each letter in the current word
            while (isalpha(*(text + i))) {
                // increment the word length counter and move to the next letter
                word_len++;
                i++;
            }

            // Check if the current index marks the end of a sentence
            if (count_sentences(text + i)) {
                // Allocate memory for the current word in the output array
                *(*(*output + k) + j) = malloc(sizeof(char ) * (word_len + 1));
                if (*(*(*output + k) + j) == NULL) {
                    destroy(*output);
                    *output = NULL;
                    return 3;
                }

                // Copy the current word from the input text into the output array
                strncpy(*(*(*output + k) + j), text + i - word_len, word_len);
                *(*(*(*output + k) + j) + word_len) = '\0';

                // Increment the word count index
                j++;
            }

        } else {
            // Increment the char count index
            i++;
        }
    }

    return 0;
}

int count_words(char **sentence) {
    int word_count = 0;

    for (int i = 0; *(sentence + i) != NULL; i++) {
        word_count++;
    }

    return word_count;
}

// function to sort sentences using strcmp function
int sort_sentences(char ***output) {
    if (output == NULL || *output == NULL) {
        return 1;
    }

    int number_of_sentences = 0;

    while (*(output + number_of_sentences)) {
        number_of_sentences++;
    }

    // sort sentences based on word count and then alphabetically
    for (int i = 0; i < number_of_sentences; i++) {
        for (int j = 0; j < number_of_sentences - i - 1; j++) {
            if (count_words(*(output + j)) > count_words(*(output + j + 1))) {
                char **temp_sentence = *(output + j);
                *(output + j) = *(output + j + 1);
                *(output + j + 1) = temp_sentence;
            }
        }
    }

    for (int i = 0; i < number_of_sentences; i++) {
        int size = count_words(*(output + i));
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size - j - 1; k++) {
                if (strcmp(*(*(output + i) + k), *(*(output + i) + k + 1)) > 0) {
                    char *temp_word = *(*(output + i) + k);
                    *(*(output + i) + k) = *(*(output + i) + k + 1);
                    *(*(output + i) + k + 1) = temp_word;
                }
            }
        }
    }


    return 0;
}


int main() {
    char *text = malloc(sizeof(char) * 1000);
    if (text == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    printf("Enter text:");
    fgets(text, 1000, stdin);

    int number_of_sentences = count_sentences(text);
    if (number_of_sentences == 0) {
        printf("Nothing to show");
        free(text);
        return 0;
    }

    char ***output = NULL;
    int check = split_sentences(text, &output);
    if (check == 2) {
        printf("Nothing to show");
        free(text);
        return 0;
    }

    if (check == 3) {
        printf("Failed to allocate memory");
        free(text);
        return 8;
    }

    free(text);

    int i, j;
    sort_sentences(output);

    for (i = 0; i < number_of_sentences; i++) {
        j = 0;
        if (*(*(output + i) + j) == NULL) {
            printf("Nothing to show");
        } else {
            while (*(*(output + i) + j) != NULL) {
                printf("%s ", *(*(output + i) + j));
                j++;
            }
        }
        printf("\n");
    }


    destroy(output);

    return 0;
}
