//
// Created by frahmt on 27.05.21.
//

#ifndef C_DECODE_H
#define C_DECODE_H

typedef struct {
    unsigned short *const buffer;
    short head;
    short tail;
    const short maxidx;
} circ_buff_t;


short shortFromBits(unsigned const short* bits, short len);
void _decode_lookup(const unsigned short *bits);
void _decode(const unsigned short *bits);
void decode(unsigned short bit);
void circ_buff_push(circ_buff_t *c, unsigned short data);


#endif //C_DECODE_H
