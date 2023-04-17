#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image_utils.h"

int check_file_type(const char* filename, const char* suffix) {
    unsigned long long filename_len = strlen(filename);
    unsigned long long suffix_len = strlen(suffix);

    if (filename_len < suffix_len) {
        return 0;
    }

    const char* end = filename + (filename_len - suffix_len);
    return strcmp(end, suffix) == 0;
}

struct image_t* load_image_t(const char *filename, int *err_code) {
    if (filename == NULL) {
        if (err_code != NULL)
            *err_code = 1;
        return NULL;
    }

    FILE *fp;
    int width, height;
    char *type = calloc(3, sizeof(char));

    if (check_file_type(filename, ".txt") || check_file_type(filename, ".bin")) {
        fp = fopen(filename, "r");
        if (fp == NULL) {
            free(type);
            if (err_code != NULL)
                *err_code = 2;
            return NULL;
        }

        if (type == NULL) {
            fclose(fp);
            if (err_code != NULL)
                *err_code = 4;
            return NULL;
        }

        if (fscanf(fp, "%2s", type) != 1) {
            fclose(fp);
            free(type);
            if (err_code != NULL)
                *err_code = 3;
            return NULL;
        }

        if (!strstr(type, "P2")) {
            fclose(fp);
            free(type);
            if (err_code != NULL)
                *err_code = 3;
            return NULL;
        }

        if (fscanf(fp, "%d %d", &width, &height) != 2) {
            fclose(fp);
            free(type);
            if (err_code != NULL) {
                *err_code = 3;
            }
            return NULL;
        }
    } else {
        if (err_code != NULL) {
            *err_code = 1;
        }
        free(type);
        return NULL;
    }

    if (height <= 0 || width <= 0) {
        fclose(fp);
        free(type);
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }

    struct image_t *img = calloc(1, sizeof(struct image_t));
    if (img == NULL) {
        if (err_code != NULL) {
            *err_code = 4;
        }
        free(type);
        fclose(fp);
        return NULL;
    }

    strcpy(img->type, type);

    img->ptr = calloc(height,  sizeof(int *));

    if (img->ptr == NULL) {
        free(img);
        fclose(fp);
        free(type);
        if (err_code != NULL) {
            *err_code = 4;
        }
        return NULL;
    }

    for(int i=0; i<height; i++){
        *(img->ptr + i) = malloc(sizeof(int) * width);
        if(*(img->ptr + i) == NULL){
            for(int j=0; j<i; j++){
                free(*(img->ptr + j));
            }
            free(img->ptr);
            free(img);
            free(type);
            fclose(fp);
            if(err_code != NULL)
                *err_code = 4;
            return NULL;
        }
    }

    int max;
    fscanf(fp, "%d", &max);

    img->height = height;
    img->width = width;

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            if(fscanf(fp, "%d", (*(img->ptr + i) + j)) != 1){
                destroy_image(&img);
                free(type);
                fclose(fp);
                if(err_code != NULL)
                    *err_code = 3;
                return NULL;
            }

            if(*(*(img->ptr + i) + j) < 0 || *(*(img->ptr + i) + j) > 255){
                destroy_image(&img);
                free(type);
                fclose(fp);
                if(err_code != NULL)
                    *err_code = 3;
                return NULL;
            }
        }
    }

    if(err_code != NULL)
        *err_code = 0;
    fclose(fp);
    free(type);
    return img;
}

int save_image_t(const char * filename, const struct image_t *m1)
{
    if(filename == NULL || m1 == NULL || m1->ptr == NULL || m1->height <= 0 || m1->width <= 0){
        return 1;
    }

    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        return 2;
    }

    if(fprintf(fp, "%s\n", m1->type) <= 0){
        fclose(fp);
        return 3;
    }

    if(fprintf(fp, "%d %d\n", m1->width, m1->height) <= 0){
        fclose(fp);
        return 3;
    }

    if(fprintf(fp,"%d\n", 255) <= 0){
        fclose(fp);
        return 3;
    }

    for(int i=0; i<m1->height; i++){
        for(int j=0; j<m1->width; j++){
            if(fprintf(fp, "%d ", *(*(m1->ptr + i) + j)) <= 0){
                fclose(fp);
                return 3;
            }
        }
        if(fprintf(fp, "\n") <= 0){
            fclose(fp);
            return 3;
        }
    }

    fclose(fp);
    return 0;
}


void destroy_image(struct image_t **m) {
    if (*m == NULL) {
        return;
    }
    for (int i = 0; i < (*m)->height; i++) {
        if ((*(m))->ptr != NULL) {
            free(*((*m)->ptr + i));
        }
    }
    free((*m)->ptr);
    free(*m);
    *m = NULL;
}



const int *image_get_pixel(const struct image_t *img, int x, int y) {
    if (img == NULL || img->ptr == NULL || x < 0 || x >= img->width || y < 0 || y >= img->height)
        return NULL;
    return &(*(*(img->ptr + y) + x));
}

int *image_set_pixel(struct image_t *img, int x, int y) {
    if (img == NULL || img->ptr == NULL || x < 0 || x >= img->width || y < 0 || y >= img->height)
        return NULL;

    /*if(strcmp(img->type, "P1") == 0){
        if(*(*(img->ptr + y) + x) == 0){
            *(*(img->ptr + y) + x) = 1;
        }
    }

    else if(strcmp(img->type, "P2") == 0){
        if (*(*(img->ptr + y) + x) == 0) {
            *(*(img->ptr + y) + x) = 255;
        }
    }

    else if(strcmp(img->type, "P3") == 0){
        //int pixel_index = (y * img->width + x) * 3;
        if (*(img->ptr + (y * img->width + x) * 3) == 0 && *(img->ptr + (y * img->width + x) * 3+1) == 0 && *(img->ptr + (y * img->width + x) * 3+2) == 0) {
            *(img->ptr + (y * img->width + x) * 3) = (int *) 255;
            *(img->ptr + (y * img->width + x) * 3) = (int *) 255;
            *(img->ptr + (y * img->width + x) * 3) = (int *) 255;
        }
    }

    else{
        return NULL;
    }*/

    return &(*(*(img->ptr + y) + x));
}


int draw_image(struct image_t *img, const struct image_t *src, int destx, int desty) {
    if (img == NULL || src == NULL || destx < 0 || desty < 0 || destx + src->width > img->width || desty + src->height > img->height || src->width <= 0 || src->height <= 0) {
        return 1;
    }

    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            int *dest_pixel = image_set_pixel(img, destx + x, desty + y);
            const int *src_pixel = image_get_pixel(src, x, y);
            if (dest_pixel == NULL || src_pixel == NULL) {
                return 1;
            }
            *dest_pixel = *src_pixel;
        }
    }

    return 0;
}