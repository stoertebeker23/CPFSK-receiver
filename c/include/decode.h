//
// Created by frahmt on 27.05.21.
//

#ifndef C_DECODE_H
#define C_DECODE_H

void _decode(const unsigned char *bits);
void decode(unsigned char bit);
void circ_buff_push(circ_buff_t *c, short data);

unsigned char const NUM_CHAR[5] = {1, 0, 1, 1, 1};
unsigned char const LET[5] = {1, 1, 1, 1, 1};

unsigned char const A[5] = {0, 0, 0, 1, 1};
unsigned char const B[5] = {1, 1, 0, 0, 1};
unsigned char const C[5] = {0, 1, 1, 1, 0};
unsigned char const D[5] = {0, 1, 0, 0, 1};
unsigned char const E[5] = {0, 0, 0, 0, 1};
unsigned char const F[5] = {0, 1, 1, 0, 1};
unsigned char const G[5] = {1, 1, 0, 1, 0};
unsigned char const H[5] = {1, 0, 1, 0, 0};
unsigned char const I[5] = {0, 0, 1, 1, 0};
unsigned char const J[5] = {0, 1, 0, 1, 1};
unsigned char const K[5] = {0, 1, 1, 1, 1};
unsigned char const L[5] = {1, 0, 0, 1, 0};
unsigned char const M[5] = {1, 1, 1, 0, 0};
unsigned char const N[5] = {0, 1, 1, 0, 0};
unsigned char const O[5] = {1, 1, 0, 0, 0};
unsigned char const P[5] = {1, 0, 1, 1, 0};
unsigned char const Q[5] = {1, 0, 1, 1, 1};
unsigned char const R[5] = {0, 1, 0, 1, 0};
unsigned char const S[5] = {0, 0, 1, 0, 1};
unsigned char const T[5] = {1, 0, 0, 0, 0};
unsigned char const U[5] = {0, 0, 1, 1, 1};
unsigned char const V[5] = {1, 1, 1, 1, 0};
unsigned char const W[5] = {1, 0, 0, 1, 1};
unsigned char const X[5] = {1, 1, 1, 0, 1};
unsigned char const Y[5] = {1, 0, 1, 0, 1};
unsigned char const Z[5] = {1, 0, 0, 0, 1};
unsigned char const CR[5] = {0, 1, 0, 0, 0};
unsigned char const TAB[5] = {0, 0, 0, 1, 0};
unsigned char const SPACE[5] = {0, 0, 1, 0, 0};

typedef struct {
    unsigned char *const buffer;
    short head;
    short tail;
    const short maxlen;
} circ_buff_t;


#endif //C_DECODE_H
