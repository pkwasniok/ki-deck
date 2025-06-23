#include <stdio.h>

#include "utils/delay.h"

void app_main(void) {
    while (1) {
        printf("Hello, world!\n");
        delay_s(1);
    }
}

