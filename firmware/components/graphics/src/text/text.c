#include "graphics/text.h"
#include "graphics/geometry.h"

#include <stdio.h>
#include <string.h>

void bdf_parse_header(FILE* file);

void g_font_load(const char *filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error\n");
        return;
    }

    bdf_parse_header(file);

    fclose(file);
}

void bdf_parse_header(FILE* file) {
    char buffer[256];

    // File should start with "STARTFONT..."
    fgets(buffer, 256, file);
    if (strncmp(buffer, "STARTFONT", 9) != 0) {
        printf("Error\n");
        return;
    }

    int found_font = 0;
    int found_size = 0;
    int found_fontboundingbox = 0;

    for (int i = 0; i < 3; i++) {
        fgets(buffer, 256, file);

        if (strncmp(buffer, "FONT ", 5) == 0) {
            found_font = 1;
        } else if (strncmp(buffer, "SIZE ", 5) == 0) {
            found_size = 1;
        } else if (strncmp(buffer, "FONTBOUNDINGBOX ", 16) == 0) {
            found_fontboundingbox = 1;
        } else if (strncmp(buffer, "COMMENT ", 8) == 0) {
            i -= 1;
        }
    }

    if (!found_font || !found_size || !found_fontboundingbox) {
        printf("Error\n");
        return;
    }
}

