#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <include/decode.h>

#define BUFF_LEN 5


void circ_buff_push(circ_buff_t *c, short data) {
    short next_elm;
    next_elm = c->head + 1;        // next_elm is where head will point to after this write.
    if (next_elm >= c->maxlen)
        next_elm = 0;

    if (next_elm == c->tail)        // if the head + 1 == tail, -> buffer full
        next_elm = c->tail;

    c->buffer[c->head] = data;    // Load data and then move
    c->head = next_elm;        // head to next_elm data offset.
}

void decode(unsigned char bit) {

    unsigned char const start_stop[BUFF_LEN] = {0, 0, 1, 1, 1};
    static unsigned char buff[BUFF_LEN] = {0, 0, 0, 0, 0};
    static short cnt = 0;
    static bool transfer = 0;
    static circ_buff_t cbuff = {.buffer = buff, .head = 0, .tail = 0, .maxlen =
    5};

    circ_buff_push(&cbuff, bit);
    if (memcmp(cbuff.buffer, start_stop, 5) == 0 && !transfer) {
        // if detected, wait for next #buff_len symbols
        transfer = 1;
    }

    if (transfer) {
        cnt++;
        if (cnt >= BUFF_LEN) {
            _decode(cbuff.buffer);
            transfer = 0;
            cnt = 0;
        }
    }

}


void _decode(const unsigned char *bits) {
    static bool let_num = 0; // default are letters

    if (memcmp(bits, NUM_CHAR, 5) == 0 || memcmp(bits, LET, 5) == 0) {
        let_num = !let_num;
    }

    if (memcmp(bits, A, 5) == 0) {
        if (!let_num) {
            printf("A");
        } else {
            printf("-");
        }
    } else if (memcmp(bits, B, 5) == 0) {
        if (!let_num) {
            printf("B");
        } else {
            printf("?");
        }
    } else if (memcmp(bits, C, 5) == 0) {
        if (!let_num) {
            printf("C");
        } else {
            printf(":");
        }
    } else if (memcmp(bits, D, 5) == 0) {
        if (!let_num) {
            printf("D");
        } else {
            printf("Wer da?");
        }
    } else if (memcmp(bits, E, 5) == 0) {
        if (!let_num) {
            printf("E");
        } else {
            printf("3");
        }
    } else if (memcmp(bits, F, 5) == 0) {
        printf("F");
    } else if (memcmp(bits, G, 5) == 0) {
        printf("G");
    } else if (memcmp(bits, H, 5) == 0) {
        printf("H");
    } else if (memcmp(bits, I, 5) == 0) {
        if (!let_num) {
            printf("I");
        } else {
            printf("8");
        }
    } else if (memcmp(bits, J, 5) == 0) {
        if (!let_num) {
            printf("J");
        } else {
            printf("Klingel");
        }
    } else if (memcmp(bits, K, 5) == 0) {
        if (!let_num) {
            printf("K");
        } else {
            printf("(");
        }
    } else if (memcmp(bits, L, 5) == 0) {
        if (!let_num) {
            printf("L");
        } else {
            printf(")");
        }
    } else if (memcmp(bits, M, 5) == 0) {
        if (!let_num) {
            printf("M");
        } else {
            printf(".");
        }
    } else if (memcmp(bits, N, 5) == 0) {
        if (!let_num) {
            printf("N");
        } else {
            printf(",");
        }
    } else if (memcmp(bits, O, 5) == 0) {
        if (!let_num) {
            printf("O");
        } else {
            printf("9");
        }
    } else if (memcmp(bits, P, 5) == 0) {
        if (!let_num) {
            printf("P");
        } else {
            printf("0");
        }
    } else if (memcmp(bits, Q, 5) == 0) {
        if (!let_num) {
            printf("Q");
        } else {
            printf("1");
        }
    } else if (memcmp(bits, R, 5) == 0) {
        if (!let_num) {
            printf("R");
        } else {
            printf("4");
        }
    } else if (memcmp(bits, S, 5) == 0) {
        if (!let_num) {
            printf("S");
        } else {
            printf("'");
        }
    } else if (memcmp(bits, T, 5) == 0) {
        if (!let_num) {
            printf("T");
        } else {
            printf("5");
        }
    } else if (memcmp(bits, U, 5) == 0) {
        if (!let_num) {
            printf("U");
        } else {
            printf("7");
        }
    } else if (memcmp(bits, V, 5) == 0) {
        if (!let_num) {
            printf("V");
        } else {
            printf("=");
        }
    } else if (memcmp(bits, W, 5) == 0) {
        if (!let_num) {
            printf("W");
        } else {
            printf("2");
        }
    } else if (memcmp(bits, X, 5) == 0) {
        if (!let_num) {
            printf("X");
        } else {
            printf("/");
        }
    } else if (memcmp(bits, Y, 5) == 0) {
        if (!let_num) {
            printf("Y");
        } else {
            printf("6");
        }
    } else if (memcmp(bits, Z, 5) == 0) {
        if (!let_num) {
            printf("Z");
        } else {
            printf("+");
        }
    } else if (memcmp(bits, CR, 5) == 0) {
        printf("\n");
    } else if (memcmp(bits, TAB, 5) == 0) {
        printf("\t");
    } else if (memcmp(bits, SPACE, 5) == 0) {
        printf(" ");
    } else {
        for (int x = 0; x < 5; x++) {
            printf("%d ", bits[x]);
        }
        printf("\n pattern not recognized");
    }
}

int main() {
    int array[40] = {0, 0, 1, 1, 1, // stop start (ss)
                     0, 0, 0, 1, 1, // A
                     0, 0, 1, 1, 1, // ss
                     0, 1, 1, 0, 1, // K
                     0, 0, 1, 1, 1, // ss
                     0, 1, 0, 0, 0, // cr
                     0, 0, 1, 1, 1, // ss
                     1, 0, 0, 0, 1}; // Z

    for (int loop = 0; loop < 40; loop++)
        decode(array[loop]);

    return 0;
}
