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

unsigned short NUM_CHAR[5] = {1, 1, 0, 1, 1};
unsigned short LET[5] = {1, 1, 1, 1, 1};

unsigned short const A[5] = {0, 0, 0, 1, 1};
unsigned short const B[5] = {1, 1, 0, 0, 1};
unsigned short const C[5] = {0, 1, 1, 1, 0};
unsigned short const D[5] = {0, 1, 0, 0, 1};
unsigned short const E[5] = {0, 0, 0, 0, 1};
unsigned short const F[5] = {0, 1, 1, 0, 1};
unsigned short const G[5] = {1, 1, 0, 1, 0};
unsigned short const H[5] = {1, 0, 1, 0, 0};
unsigned short const I[5] = {0, 0, 1, 1, 0};
unsigned short const J[5] = {0, 1, 0, 1, 1};
unsigned short const K[5] = {0, 1, 1, 1, 1};
unsigned short const L[5] = {1, 0, 0, 1, 0};
unsigned short const M[5] = {1, 1, 1, 0, 0};
unsigned short const N[5] = {0, 1, 1, 0, 0};
unsigned short const O[5] = {1, 1, 0, 0, 0};
unsigned short const P[5] = {1, 0, 1, 1, 0};
unsigned short const Q[5] = {1, 0, 1, 1, 1};
unsigned short const R[5] = {0, 1, 0, 1, 0};
unsigned short const S[5] = {0, 0, 1, 0, 1};
unsigned short const T[5] = {1, 0, 0, 0, 0};
unsigned short const U[5] = {0, 0, 1, 1, 1};
unsigned short const V[5] = {1, 1, 1, 1, 0};
unsigned short const W[5] = {1, 0, 0, 1, 1};
unsigned short const X[5] = {1, 1, 1, 0, 1};
unsigned short const Y[5] = {1, 0, 1, 0, 1};
unsigned short const Z[5] = {1, 0, 0, 0, 1};

unsigned short const CR[5] = {0, 1, 0, 0, 0};
unsigned short const TAB[5] = {0, 0, 0, 1, 0};
unsigned short const SPACE[5] = {0, 0, 1, 0, 0};


#endif //C_DECODE_H
