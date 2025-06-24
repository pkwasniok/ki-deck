#pragma once

typedef enum {
    G_IMAGE_FORMAT_PBM,
} g_image_format_t;

void g_image(int x0, int y0, const char* filename, g_image_format_t format);

