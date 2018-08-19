#ifndef GRAPHICS_C
#define GRAPHICS_C

#include "config.h"
#include "cutils.h"
#include "state.c"

void print_block(int i, int j, char c) {
    gotoxy(j, i+1);
    switch (c) {
        case '.':
            setformat(7,6,0);
            printf(" ");
            break;
        case 'P':
            setformat(5,5,0);
            printf(" ");
            break;
        case 'C':
            setformat(3,3,0);
            printf(" ");
            break;
        case '#':
            setformat(1,1,0);
            printf(" ");
            break;
        case '?':
            setformat(7,1,0);
            printf("?");
            break;
        default:
            setformat(7,4,0);
            printf("X");
            break;
    }
    fflush(stdout);
}

void draw_board(struct GameState* gs) {
    int i,j;
    clrscr();
    gotoxy(0,0);
    printf("SCORE %-6.1d COINS %-2.1d LIVES %-2.1d\n",gs->score,gs->coins,gs->lives);
    for (i=0; i<SCREEN_HEIGHT; i++) {
        for (j=0; j<gs->map_width; j++) {
            print_block(i,j,gs->board[i][j]);
        }
    }
}

#endif