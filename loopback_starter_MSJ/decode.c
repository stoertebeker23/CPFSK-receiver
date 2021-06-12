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
short indexo = 0;
short real_index = 0;
short distance;
const short start_stop[2] = {1,0};
void circ_buff_push(circ_buff_t *c, unsigned short data) {
    short next_elm;
    next_elm = c->head + 1;         // next_elm is where head will point to after this write.

    if (c->head == c->maxidx)
        next_elm = c->tail;

    c->buffer[c->head] = data;      // Load data and then move
    c->head = next_elm;             // head to next_elm data offset.
}


void decode(unsigned short bit) {
    if (!current_lut) 
        current_lut = lookup_num;
    buffer = buffer << 1;
    buffer = buffer | bit;
    startstop = buffer & 0x001F;
    distance -= 1;
    if (startstop == 28 && distance < 0) {
        distance = 10;
        indexo = (buffer >> 5) & 0x03FF;
        real_index = (((indexo >> 0) & 0x0001) << 4) | (((indexo >> 2) & 0x0001) << 3) | (((indexo >> 4) & 0x0001) << 2) | (((indexo >> 6) & 0x0001) << 1) | (((indexo >> 8) & 0x0001) << 0);
        if (real_index == 31) {
            current_lut = lookup_char;
        } else if (real_index == 27) {
            current_lut = lookup_num;
        } else {
            printf(" >> %c <<\n",current_lut[real_index-1]);
        }
    }
}

short shortFromBits(unsigned const short *bits, short len) {
    short res = 0;
    for (int i = (len - 1); i >= 0; --i) {
        if (bits[i] == 1) {
            res |= 1 << (i);
        }
    }
    return res;
}

void _decode_lookup(const unsigned short *bits) {

    static char *lookup = NULL;
    if (lookup == NULL) {
        lookup = lookup_char;
    }
    static bool NUM = false;
    static short cnt = 1;
    short idx = shortFromBits(bits, 5); // get index
    short _idx = 0; // tmp idx
    if (idx == 31) {
        // Letters
        NUM = false;
        lookup = lookup_char;
    } else if (idx == 27) {
        // Numbers
        NUM = true;
        lookup = lookup_num;
    } else if (NUM) {
        if (idx == 9 || idx == 11) {
            for (int i = (idx + (cnt - 1) * OFFSET); i <= (idx + cnt * OFFSET); ++i) {
                printf("%c", *(lookup + (i - 1)));
            }
            printf("\n");
            cnt++;
        } else if (idx > 9) {
            _idx = (idx + OFFSET);
            if (idx > 11) {
                _idx += OFFSET;
                if (idx > 12) {
                    _idx -= 1;
                    if (idx > 20) {
                        _idx -= 1;
                    }
                }
                printf("%c", *(lookup + (_idx)));
            } else {
                printf("%c", *(lookup + (_idx - 1)));
            }
        } else {
            printf("%c", *(lookup + (idx - 1)));
        }
    } else {
        printf("%c", *(lookup + (idx - 1)));
    }
}
/*
void _decode(const unsigned short bits[5]) {
    static bool let_num = 0; // default are letters
    if (memcmp(bits, NUM_CHAR, 5) == 0 || memcmp(bits, LET, 5) == 0) {
        let_num = !let_num;
    }

    if (memcmp(bits, A, sizeof(A)) == 0) {
        if (!let_num) {
            printf("A");
        } else {
            printf("-");
        }
    } else if (memcmp(bits, B, sizeof(B)) == 0) {
        if (!let_num) {
            printf("B");
        } else {
            printf("?");
        }
    } else if (memcmp(bits, C, sizeof(C)) == 0) {
        if (!let_num) {
            printf("C");
        } else {
            printf(":");
        }
    } else if (memcmp(bits, D, sizeof(D)) == 0) {
        if (!let_num) {
            printf("D");
        } else {
            printf("Wer da?");
        }
    } else if (memcmp(bits, E, sizeof(E)) == 0) {
        if (!let_num) {
            printf("E");
        } else {
            printf("3");
        }
    } else if (memcmp(bits, F, sizeof(F)) == 0) {
        printf("F");
    } else if (memcmp(bits, G, sizeof(G)) == 0) {
        printf("G");
    } else if (memcmp(bits, H, sizeof(H)) == 0) {
        printf("H");
    } else if (memcmp(bits, II, 5) == sizeof(II)) {
        if (!let_num) {
            printf("I");
        } else {
            printf("8");
        }
    } else if (memcmp(bits, J, sizeof(J)) == 0) {
        if (!let_num) {
            printf("J");
        } else {
            printf("Klingel");
        }
    } else if (memcmp(bits, K, sizeof(K)) == 0) {
        if (!let_num) {
            printf("K");
        } else {
            printf("(");
        }
    } else if (memcmp(bits, L, sizeof(L)) == 0) {
        if (!let_num) {
            printf("L");
        } else {
            printf(")");
        }
    } else if (memcmp(bits, M, sizeof(M)) == 0) {
        if (!let_num) {
            printf("M");
        } else {
            printf(".");
        }
    } else if (memcmp(bits, N, sizeof(N)) == 0) {
        if (!let_num) {
            printf("N");
        } else {
            printf(",");
        }
    } else if (memcmp(bits, O, sizeof(O)) == 0) {
        if (!let_num) {
            printf("O");
        } else {
            printf("9");
        }
    } else if (memcmp(bits, P, sizeof(P)) == 0) {
        if (!let_num) {
            printf("P");
        } else {
            printf("0");
        }
    } else if (memcmp(bits, Q, sizeof(Q)) == 0) {
        if (!let_num) {
            printf("Q");
        } else {
            printf("1");
        }
    } else if (memcmp(bits, R, sizeof(R)) == 0) {
        if (!let_num) {
            printf("R");
        } else {
            printf("4");
        }
    } else if (memcmp(bits, S, sizeof(S)) == 0) {
        if (!let_num) {
            printf("S");
        } else {
            printf("'");
        }
    } else if (memcmp(bits, T, sizeof(T)) == 0) {
        if (!let_num) {
            printf("T");
        } else {
            printf("5");
        }
    } else if (memcmp(bits, U, sizeof(U)) == 0) {
        if (!let_num) {
            printf("U");
        } else {
            printf("7");
        }
    } else if (memcmp(bits, V, sizeof(V)) == 0) {
        if (!let_num) {
            printf("V");
        } else {
            printf("=");
        }
    } else if (memcmp(bits, W, sizeof(W)) == 0) {
        if (!let_num) {
            printf("W");
        } else {
            printf("2");
        }
    } else if (memcmp(bits, X, sizeof(X)) == 0) {
        if (!let_num) {
            printf("X");
        } else {
            printf("/");
        }
    } else if (memcmp(bits, Y, sizeof(X)) == 0) {
        if (!let_num) {
            printf("Y");
        } else {
            printf("6");
        }
    } else if (memcmp(bits, Z, sizeof(Z)) == 0) {
        if (!let_num) {
            printf("Z");
        } else {
            printf("+");
        }
    } else if (memcmp(bits, CR, sizeof(CR)) == 0) {
        printf("\n");
    } else if (memcmp(bits, TAB, sizeof(TAB)) == 0) {
        printf("\t");
    } else if (memcmp(bits, SPACE, sizeof(SPACE)) == 0) {
        printf(" ");
    } else {
        for (int x = 0; x < 5; x++) {
            printf("%d ", bits[x]);
        }
        printf("\n pattern not recognized");
    }
}
*/
/*
int main() {

    unsigned short array[110] = {0, 0, 1, 1, 1, // stop start (ss)
                                1, 1, 0, 1, 1, // num
                                0, 0, 1, 1, 1, // ss
                                1, 0, 0, 0, 0, // 3
                                0, 0, 1, 1, 1,
                                0, 0, 0, 1, 0, // \n
                                0, 0, 1, 1, 1, // ss
                                1, 0, 0, 1, 0, // wer da
                                0, 0, 1, 1, 1,
                                0, 0, 0, 1, 0, // \n
                                0, 0, 1, 1, 1, // ss
                                0, 1, 1, 1, 1,
                                0, 0, 1, 1, 1,
                                0, 0, 0, 1, 0, // \n
                                0, 0, 1, 1, 1, // ss
                                1, 1, 0, 1, 0,
                                0, 0, 1, 1, 1, // ss
                                1, 1, 1, 1, 1, // switch
                                0, 0, 1, 1, 1, // ss
                                1, 0, 0, 0, 0, // E
    }; //=



    for (int loop = 0; loop < 110; loop++) {
        decode(array[loop]);
    }

    return 0;
}
*/