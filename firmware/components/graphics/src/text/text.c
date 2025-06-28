#include "graphics/text.h"
#include "graphics/graphics.h"

#include <stdio.h>
#include <string.h>




void render_glyph(int x0, int y0, g_glyph_t* glyph);

void g_text(int x0, int y0, char* text, g_font_t *font) {
    int x = 0;

    for (int i = 0; i < strlen(text); i++) {
        for (int j = 0; j < font->length; j++) {
            if (text[i] == font->glyphs[j].code) {
                render_glyph(x0 + x + font->offset_x, y0 + font->height - font->glyphs[j].height - font->offset_y, &font->glyphs[j]);
                x += font->glyphs[j].shift;
                break;
            }
        }
    }
}

void render_glyph(int x0, int y0, g_glyph_t* glyph) {
    int bytes_per_row = (glyph->width + 7) / 8;

    for (int y = 0; y < glyph->height; y++) {
        for (int x = 0; x < glyph->width; x++) {
            int byte_number = (y * bytes_per_row) + (x / 8);
            int bit_number = 7 - (x % 8);

            if (glyph->buffer[byte_number] & (1 << bit_number)) {
                G_MATRIX_SET(x0 + x + glyph->offset_x, y0 + y - glyph->offset_y);
            }
        }
    }
}





void bdf_load(FILE* file, g_font_t* font);

void g_font_load(const char *filename, g_font_format_t format, g_font_t* font) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error\n");
        return;
    }

    switch (format) {
        case G_FONT_FORMAT_BDF:
            bdf_load(file, font);
            break;
    }

    fclose(file);
}

int bdf_parse_header(FILE* file);
int bdf_parse_properties(FILE* file, g_font_t* font);
int bdf_parse_glyphs(FILE* file, g_font_t* font);
int bdf_parse_glyph(FILE* file, g_glyph_t* glyph);

void bdf_load(FILE* file, g_font_t* font) {
    int ret;

    // Parse BDF file header
    ret = bdf_parse_header(file);
    if (ret != G_FONT_OK) {
        printf("Error\n");
        return;
    }

    // Parse BDF file properties
    ret = bdf_parse_properties(file, font);
    if (ret != G_FONT_OK) {
        printf("Error\n");
        return;
    }

    // Allocate resources
    int bytes_per_glyph = ((font->width + 7) / 8) * font->height;
    font->glyphs = calloc(font->length, sizeof(g_glyph_t));
    font->buffer = calloc(font->length, bytes_per_glyph);

    // Assign font buffer segments to glyphs
    for (int i = 0; i < font->length; i++) {
        font->glyphs[i].buffer = font->buffer + (i * bytes_per_glyph);
    }

    // Parse glyphs
    bdf_parse_glyphs(file, font);
}

int bdf_parse_header(FILE* file) {
    char buffer[128];

    fgets(buffer, 128, file);
    if (strncmp(buffer, "STARTFONT", 9) != 0) {
        return G_FONT_INVALID;
    }

    return G_FONT_OK;
}

int bdf_parse_properties(FILE* file, g_font_t* font) {
    char buffer[128];

    rewind(file);

    while (fgets(buffer, 128, file) != NULL) {
        if (strncmp(buffer, "FONTBOUNDINGBOX ", 16) == 0) {
            char* ptr = strtok(buffer + 16, " ");
            font->width = atoi(ptr);

            ptr = strtok(NULL, " ");
            font->height = atoi(ptr);

            ptr = strtok(NULL, " ");
            font->offset_x = atoi(ptr);

            ptr = strtok(NULL, " ");
            font->offset_y = atoi(ptr);
        } else if (strncmp(buffer, "CHARS ", 6) == 0) {
            font->length = atoi(buffer + 6);
            break;
        }
    }

    return G_FONT_OK;
}

int bdf_parse_glyphs(FILE* file, g_font_t* font) {
    assert(font != NULL);
    assert(font->length > 0 && font->length < 256);
    assert(font->glyphs != NULL);
    assert(font->buffer != NULL);

    rewind(file);

    for (int i = 0; i < font->length; i++) {
        bdf_parse_glyph(file, &font->glyphs[i]);
    }

    return G_FONT_OK;
}

int bdf_parse_glyph(FILE* file, g_glyph_t* glyph) {
    assert(glyph != NULL);

    char buffer[128];

    // Find "STARTCHAR"
    while (fgets(buffer, 128, file) != NULL) {
        if (strncmp(buffer, "STARTCHAR", 9) == 0) {
            break;
        }
    }

    int parse_bitmap = 0;
    int current_byte = 0;
    while (fgets(buffer, 128, file) != NULL) {
        if (strncmp(buffer, "ENDCHAR", 7) == 0) {
            break;
        }

        if (parse_bitmap) {
            int bytes_per_row = (glyph->width + 7) / 8;

            for (int i = 0; i < bytes_per_row; i++) {
                char hex_byte[3];
                hex_byte[0] = buffer[2 * i];
                hex_byte[1] = buffer[2 * i + 1];
                hex_byte[2] = '\0';

                uint8_t byte = strtol(hex_byte, NULL, 16);

                glyph->buffer[current_byte++] = byte;
            }

            continue;
        }

        if (strncmp(buffer, "ENCODING ", 9) == 0) {
            glyph->code = atoi(buffer + 9);
        } else if (strncmp(buffer, "BBX ", 4) == 0) {
            char* ptr = strtok(buffer + 4, " ");
            glyph->width = atoi(ptr);

            ptr = strtok(NULL, " ");
            glyph->height = atoi(ptr);

            ptr = strtok(NULL, " ");
            glyph->offset_x = atoi(ptr);

            ptr = strtok(NULL, " ");
            glyph->offset_y = atoi(ptr);
        } else if (strncmp(buffer, "DWIDTH ", 7) == 0) {
            glyph->shift = atoi(buffer + 7);
        } else if (strncmp(buffer, "BITMAP", 6) == 0) {
            parse_bitmap = 1;
        }
    }

    return G_FONT_OK;
}

