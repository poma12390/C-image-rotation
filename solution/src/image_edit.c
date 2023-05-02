#include "image.h"
#include "image_edit.h"

struct image rotate(struct image img) {
    struct image blank = create_blank_image(img.height, img.width);

    for (size_t i = 0; i < blank.height; i++) {
        for(size_t j = 0; j < blank.width; j ++)
            *(blank.data + blank.width * i + j) = *(img.data + img.width * (img.height - 1 - j) + i);
    }

    return blank;
}

