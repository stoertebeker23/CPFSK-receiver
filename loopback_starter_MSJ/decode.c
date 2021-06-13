
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "include/decode.h"
#include "include/lookup.h"

#define STOP_SEQUENCE 0x001C
#define SWITCH_TO_CHAR 0x1F
#define SWITCH_TO_NUM 0x1B
#define BUFF_LEN 5
#define OFFSET 6

char * current_lut = NULL;
uint16_t buffer = 0;
uint16_t startstop = 0;
uint16_t index_pre = 0;
uint8_t real_index = 0;
uint8_t startstop_holdoff = 11;

void decode(unsigned short bit) {
    // Toughts on this:
    // Problem stream starts with numbers not characters -> no problem, at the first change everything is cleared up
    // Problem detection of wrong start stop bit: The first message that doesnt contain a 111100 will synchronize on the right start and stop bits

    if (!current_lut) 
        current_lut = lookup_char;

    buffer = buffer << 1;
    buffer = buffer | bit;
    startstop = buffer & 0x001F;

    if (startstop_holdoff > 0) // Underflow protection
        startstop_holdoff -= 1;

    if (startstop == STOP_SEQUENCE && startstop_holdoff == 0) {
        startstop_holdoff = 11;
        index_pre = (buffer >> 5) & 0x03FF; // Shift over the start and stop section and mask the 10 message bits
        // Compress bit stream to half its size
        // Turn this 1 1 1 1 0 0 1 1 0 0 into 0 1 0 1 1
        real_index = (((index_pre >> 0) & 0x0001) << 4) 
                   | (((index_pre >> 2) & 0x0001) << 3) 
                   | (((index_pre >> 4) & 0x0001) << 2) 
                   | (((index_pre >> 6) & 0x0001) << 1) 
                   | (((index_pre >> 8) & 0x0001) << 0);

        if (real_index == SWITCH_TO_CHAR) {             // Change to characters
            current_lut = lookup_char;
        } else if (real_index == SWITCH_TO_NUM) {      // Change to numbers
            current_lut = lookup_num;
        } else {
            printf(" >> %c <<\n",current_lut[real_index-1]); // Everything else is a valid character
        }
    }
}

