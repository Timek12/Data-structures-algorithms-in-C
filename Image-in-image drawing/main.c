#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_utils.h"

int main() {
    char *filename = malloc(sizeof(char) * 40);
    if (filename == NULL) {
        printf("Failed to allocate memory\n");
        return 8;
    }

    printf("Enter destination image file name:");
    if (scanf("%39s", filename) != 1) {
        printf("Incorrect input\n");
        free(filename);
        return 1;
    }


    int err_code = 0;
    struct image_t *img_out = load_image_t(filename, &err_code);
    if (img_out == NULL) {
        if (err_code == 1 || err_code == 3) {
            printf("File corrupted\n");
            free(filename);
            return 6;
        }
        else if(err_code == 2){
            printf("Couldn't open file");
            free(filename);
            return 4;
        }
        else if (err_code == 4) {
            printf("Failed to allocate memory\n");
            free(filename);
            return 8;
        }
    }

    int num_of_subimages;

    printf("How many subimages you want to draw:");
    if (scanf("%d", &num_of_subimages) != 1) {
        printf("Incorrect input");
        free(filename);
        destroy_image(&img_out);
        return 1;
    }

    if (num_of_subimages < 1) {
        printf("Incorrect input data");
        free(filename);
        destroy_image(&img_out);
        return 2;
    }


    char *filename_input = malloc(sizeof(char) * 40);
    if (filename_input == NULL) {
        printf("Failed to allocate memory");
        free(filename);
        destroy_image(&img_out);
        return 8;
    }

    int i = 0;
    while (i < num_of_subimages) {
        printf("Enter a name of a subimage:");

        if (scanf("%39s", filename_input) != 1) {
            printf("Incorrect input");
            free(filename_input);
            free(filename);
            destroy_image(&img_out);
            return 1;
        }

        err_code = 0;
        int x, y;

        struct image_t *img_in = load_image_t(filename_input, &err_code);
        if (img_in == NULL) {
            if(err_code == 2){
                printf("Couldn't open file\n");
                destroy_image(&img_in);
                i++;
                continue;
            }
            if (err_code == 3) {
                printf("File corrupted\n");
                destroy_image(&img_in);
                i++;
                continue;

            }
            else if(err_code == 1){
                destroy_image(&img_in);
                i++;
                printf("File not found\n");
                continue;
            }
            else if (err_code == 4) {
                printf("Failed to allocate memory");
                free(filename);
                free(filename_input);
                destroy_image(&img_out);
                if (img_in != NULL)
                    destroy_image(&img_in);
                return 8;
            }
        }

        printf("Enter coordinates (x, y):");
        if (scanf("%d %d", &x, &y) != 2) {
            printf("Incorrect input\n");
            free(filename);
            free(filename_input);
            destroy_image(&img_out);
            destroy_image(&img_in);
            return 1;
        }


        int check = draw_image(img_out, img_in, x, y);
        if (check) {
            printf("Incorrect input data\n");
            destroy_image(&img_in);
            i++;
            continue;
        }
        destroy_image(&img_in);
        i++;
    }

    free(filename_input);

    char *extension = strrchr(filename, '.');
    if (extension != NULL) {
        int extension_index = extension - filename;
        memmove(filename + extension_index + 9, extension, strlen(extension) + 1);
        memcpy(filename + extension_index, "_modified", 9);
    }

    int res = save_image_t(filename, img_out);
    free(filename);
    destroy_image(&img_out);
    if (res == 0) {
        printf("File saved");
        return 0;
    } else {
        printf("Couldn't create file");
        return 5;
    }
}
