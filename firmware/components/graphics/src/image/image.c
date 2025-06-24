#include "graphics/image.h"
#include "graphics/graphics.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "esp_log.h"

#define TAG "graphics/image"

void image_pbm(int x0, int y0, FILE* file);

void g_image(int x0, int y0, const char* filename, g_image_format_t format) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        ESP_LOGE(TAG, "Error occured while trying to open image file");
        return;
    }

    switch (format) {
        case G_IMAGE_FORMAT_PBM:
            image_pbm(x0, y0, file);
            break;
    }

    fclose(file);
}

int pbm_header(FILE* file);
int pbm_comment(FILE* file);
int pbm_size(FILE* file, int* width, int* height);
int pbm_data(FILE* file, uint8_t* buffer, int* length, int width, int height);

void image_pbm(int x0, int y0, FILE* file) {
    if (pbm_header(file) != 0) {
        ESP_LOGE(TAG, "Error occured while loading image. Image is invalid.");
        return;
    }

    if (pbm_comment(file) != 0) {
        ESP_LOGE(TAG, "Error occured while loading image. Image is invalid.");
        return;
    }

    int width, height;
    if (pbm_size(file, &width, &height) != 0) {
        ESP_LOGE(TAG, "Error occured while loading image. Image is invalid.");
        return;
    }

    uint8_t buffer[1024];
    int length = 1024;
    if (pbm_data(file, buffer, &length, width, height) != 0) {
        ESP_LOGE(TAG, "Error occured while loading image. Image is invalid.");
        return;
    }

    int bytes_per_row = (width + 7) / 8;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int byte_number = (y * bytes_per_row) + (x / 8);
            int bit_number = 7 - (x % 8);

            if (buffer[byte_number] & (1 << bit_number)) {
                G_MATRIX_SET(x0 + x, y0 + y);
            }
        }
    }
}

int pbm_header(FILE* file) {
    if (fgetc(file) == 'P' && fgetc(file) == '4') {
        return 0;
    }

    return 1;
}

int pbm_comment(FILE* file) {
    int c;
    int is_comment = 0;

    while ((c = fgetc(file)) != EOF) {
        if (is_comment == 1) {
            if (c == '\n') {
                is_comment = 0;
            }
        } else {
            if (isspace(c)) {
                continue;
            } else if (c == '#') {
                is_comment = 1;
            } else {
                ungetc(c, file);
                break;
            }
        }
    }

    return 0;
}

int pbm_size(FILE* file, int* width, int* height) {
    char buffer[32];

    if (fgets(buffer, 32, file) == NULL) {
        return 1;
    }

    char* ptr;

    if ((ptr = strtok(buffer, " ")) == NULL) {
        return 1;
    }

    (*width) = atoi(ptr);

    if ((ptr = strtok(NULL, " ")) == NULL) {
        return 1;
    }

    (*height) = atoi(ptr);

    return 0;
}

int pbm_data(FILE* file, uint8_t* buffer, int* length, int width, int height) {
    int i = 0;
    int c = 0;

    while ((c = fgetc(file)) != EOF && i < (*length)) {
        buffer[i++] = c;
    }

    (*length) = i;

    if (i < (width * height / 8)) {
        return 1;
    }

    return 0;
}

