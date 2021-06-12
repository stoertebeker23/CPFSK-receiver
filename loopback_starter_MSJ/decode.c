#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "include/decode.h"
#include "include/lookup.h"

#define BUFF_LEN 5
#define OFFSET 6

char * current_lut = NULL;
short buffer = 0;
short startstop = 0;
short index_pre = 0;
short real_index = 0;
short distance;

void decode(unsigned short bit) {
    if (!current_lut) 
        current_lut = lookup_char;

    buffer = buffer << 1;
    buffer = buffer | bit;
    startstop = buffer & 0x001F;
    distance -= 1;
    if (startstop == 28 && distance < 0) {
        distance = 10;
        index_pre = (buffer >> 5) & 0x03FF;
        real_index = (((index_pre >> 0) & 0x0001) << 4) | (((index_pre >> 2) & 0x0001) << 3) | (((index_pre >> 4) & 0x0001) << 2) | (((index_pre >> 6) & 0x0001) << 1) | (((index_pre >> 8) & 0x0001) << 0);
        if (real_index == 31) {
            current_lut = lookup_char;
        } else if (real_index == 27) {
            current_lut = lookup_num;
        } else {
            printf(" >> %c <<\n",current_lut[real_index-1]);
        }
    }
}

