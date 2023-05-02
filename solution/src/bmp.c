#include "image.h"
#include "bmp.h"

#define BMP_TYPE 0x4d42
#define BMP_RESERVED 0
#define BMP_SIZE 40
#define BMP_PLANES 1
#define BMP_BIT_COUNT sizeof(struct pixel) * 8
#define BMP_COMPRESSION 0
#define BMP_X_PELS_PER_METER 0
#define BMP_Y_PELS_PER_METER 0
#define BMP_CLR_USED 0
#define BMP_CLR_IMPORTANT 0

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};


static uint8_t padding(uint64_t width) {
    return (4 - (width * 3) % 4) % 4;
}
static uint32_t image_size(const struct image* bmp) {
    return bmp->height * (bmp->width * sizeof(struct pixel) + padding(bmp->width));
}
static uint32_t file_size(const struct image* bmp) {
    return image_size(bmp) + sizeof(struct bmp_header);
}
enum read_status from_bmp(FILE* file, struct image *bmp) {
    bool result;
    if (file == NULL) {
        result = true;
    } else {
        result = false;
    }
    if(result) return READ_FILE_ERROR;

    struct bmp_header header;
    if (fread(&header, sizeof(struct bmp_header), 1, file) != 1){
        return READ_HEADER_ERROR;
    }

    if (fseek(file, header.bOffBits, SEEK_SET) != 0) {
        return READ_SIGNATURE_ERROR;
    }

    *bmp = create_blank_image(header.biWidth, header.biHeight);
    uint8_t bmp_padding = padding(bmp->width);

    for (size_t i = 0; i < bmp->height; i++) {
        if (fread(bmp->data + bmp->width * i, sizeof(struct pixel),
                  bmp->width, file) != bmp->width) {
            return READ_BITS_ERROR;
        }

        if (fseek(file, bmp_padding, SEEK_CUR) != 0) {
            return READ_BITS_ERROR;
        }
    }

    return READ_SUCCESS;
}


static struct bmp_header new_header(const struct image* bmp) {
    return (struct bmp_header) {
            .bfType = BMP_TYPE,
            .bfileSize = file_size(bmp),
            .bfReserved = BMP_RESERVED,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = BMP_SIZE,
            .biWidth = bmp->width,
            .biHeight = bmp->height,
            .biPlanes = BMP_PLANES,
            .biBitCount = BMP_BIT_COUNT,
            .biCompression = BMP_COMPRESSION,
            .biSizeImage = image_size(bmp),
            .biXPelsPerMeter = BMP_X_PELS_PER_METER,
            .biYPelsPerMeter = BMP_Y_PELS_PER_METER,
            .biClrUsed = BMP_CLR_USED,
            .biClrImportant = BMP_CLR_IMPORTANT
    };
}


enum write_status to_bmp(FILE* out, struct image bmp) {
    if(!out) return WRITE_FILE_ERROR;

    struct bmp_header header = new_header(&bmp);
    fwrite(&header, sizeof(struct bmp_header), 1, out);

    if(ferror(out) !=0) {
        return WRITE_ERROR;
    }

    uint8_t bmp_padding = padding(bmp.width);

    for (size_t i = 0; i < bmp.height; i++) {
        fwrite(bmp.data + bmp.width * i, sizeof(struct pixel), bmp.width,
               out);
        const char zero_bytes[3] = {0, 0, 0};
        fwrite(zero_bytes, 1, bmp_padding, out);
    }

    return WRITE_SUCCESS;
}


