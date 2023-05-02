#ifndef IMAGE
#define IMAGE

#include <stdint.h>

#include <stdio.h>

#include <stdbool.h>

struct pixel { uint8_t b, g, r; };

struct image {
    size_t width, height;
    struct pixel* data;
};



struct image create_blank_image(size_t width, size_t height);
void free_image(struct image* img);

enum read_status  {
    READ_SUCCESS = 0,
    READ_SIGNATURE_ERROR,
    READ_BITS_ERROR,
    READ_HEADER_ERROR,
    READ_FILE_ERROR
};

enum  write_status  {
    WRITE_SUCCESS = 0,
    WRITE_ERROR,
    WRITE_FILE_ERROR
};



typedef enum read_status (from_image)(FILE*, struct image*);
typedef enum write_status (to_image)(FILE*, struct image const);


enum read_status open_file(const char* src, from_image from_img, struct image* img);

enum write_status edit_file(const char* out, to_image to_img, struct image img);

#endif
