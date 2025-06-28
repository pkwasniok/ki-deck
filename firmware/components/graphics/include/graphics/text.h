#pragma once
#include <stdint.h>

#define G_FONT_OK      0
#define G_FONT_INVALID 1

typedef enum {
    G_FONT_FORMAT_BDF,
} g_font_format_t;

typedef struct {
    int code;
    int width;
    int height;
    int offset_x;
    int offset_y;
    uint8_t* buffer;
} g_glyph_t;

typedef struct {
    int length;
    int width;
    int height;
    int offset_x;
    int offset_y;
    g_glyph_t* glyphs;
    uint8_t* buffer;
} g_font_t;

void g_font_load(const char* filename, const g_font_format_t format, g_font_t* font);

