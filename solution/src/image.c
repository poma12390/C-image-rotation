#include <malloc.h>

#include "image.h"

#include <stdbool.h>

#include <stdlib.h>

enum write_status edit_file(const char* out, to_image to_img, struct image img) {
    FILE *file = fopen(out, "wb");
    bool result;
    if (file == NULL) {
        result = true;
    } else {
        result = false;
    }
    if(result) return WRITE_FILE_ERROR;
    enum write_status status = to_img(file, img);
    fclose(file);
    return status;
}

void free_image(struct image *img) {
    if(img)
    free(img->data);
}

enum read_status open_file(const char* src, from_image from_img, struct image* img) {
    FILE* file = fopen(src, "rb");
    bool result;
    if (file == NULL) {
        result = true;
    } else {
        result = false;
    }
    if(result) return READ_FILE_ERROR;
    enum read_status status = from_img(file, img);
    fclose(file);
    return status;
}


struct image create_blank_image(size_t width, size_t height) {
    struct image image ={
            .width = width,
            .height = height,
            .data = malloc(width * height * sizeof(struct pixel))
    };
    if(!image.data){
        fprintf(stderr, "Not enough memory to create image");
        exit(-1);
    }
    return image;
}

