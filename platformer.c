#include <stdio.h>
#include <stdlib.h>
#include "cutils.h"
#include "state.c"
#include "logger.c"
#include "graphics.c"
#include "config.h"
#include "game.c"



int main(int argc, char *argv[]) {
    struct GameState* gs = malloc(sizeof(struct GameState));
    logger_init("stdout.log", "stderr.log", argc>=2 && (strcmp(argv[1],"-d")==0));
    logger_out("INIT: initialise game");
    game_init(gs, "level1", 0, 0, 3);
    draw_board(gs);
    set_conio_terminal_mode();
    cursorhide();
    game_loop(gs);
    cursorshow();
    reset_terminal_mode();
    formatreset();
    fflush(stdout);
    logger_out("INIT: closing");
    game_destroy(gs);
    //logger_destroy();
    return 0;
}