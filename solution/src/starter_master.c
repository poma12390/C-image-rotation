#include "image_edit.h"

#include "bmp.h"

#include "starter_master.h"



int start_app(char* src,  char* out){
    struct image img = (struct image){.height=0, .width=0, .data=&((struct pixel){.b=0, .r=0, .g=0})};
    enum read_status read_status = open_file(src, from_bmp, &img);
    if (read_status != READ_SUCCESS) return 1;
    struct image transformed_img = rotate(img);
    free_image(&img);
    enum write_status write_status = edit_file(out, to_bmp, transformed_img);
    free_image(&transformed_img);
    if (write_status != WRITE_SUCCESS) return 2;
    return 0;
}
