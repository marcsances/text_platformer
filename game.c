#ifndef GAME_C
#define GAME_C
#include "cutils.h"
#include "state.c"
#include "logger.c"
#include "graphics.c"
#include "config.h"
#include <sys/select.h>

#define SPEED 100

struct timeval tv_speed;
struct timeval tv_delay;

void parse_level_file(struct GameState* gs, char *map) {
    char *levelFileName, *key, *value, *msg;
    FILE *levelFile;

    levelFileName = malloc(strlen(map) + 12);
    sprintf(levelFileName, "levels/%s.lev", map);
    logger_out("LEVEL: loading level file %s",levelFileName);
    levelFile = fopen(levelFileName, "r");
    free(levelFileName);
    key = malloc(64);
    value = malloc(64);
    while (fscanf(levelFile, "%s=%s\n", key, value)!=EOF) {
        if (strcmp(key, "level")==0) {
            gs->level = value;
        } else if (strcmp(key, "next")==0) {
            gs->nextLevel = value;
        } else if (strcmp(key, "isSea")==0) {
            if (strcmp(value,"yes")==0) {
                gs->isSea = 1;
            } else {
                gs->isSea = 0;
            }
        }
        free(key);
        free(value);
        key = malloc(64);
        value = malloc(64);
    }
    free(key);
    free(value);
    fclose(levelFile);
}

int get_width_for_filesize(int size) {
    return (size/SCREEN_HEIGHT);
}

void parse_map(struct GameState* gs, char *map) {
    char *levelFileName, buf, *msg;
    FILE* mapFile;
    int size, i, x, y;
    levelFileName = malloc(strlen(map) + 12);
    sprintf(levelFileName, "levels/%s.asc", map);
    logger_out("MAP: loading map file %s",levelFileName);
    mapFile = fopen(levelFileName, "r");
    free(levelFileName);
    fseek(mapFile, 0L, SEEK_END);
    size = ftell(mapFile);
    gs->map_width = get_width_for_filesize(size);
    logger_out("MAP: file size is %d", size);
    gs->board = malloc(SCREEN_HEIGHT * sizeof(char*));
    for (i=0; i<SCREEN_HEIGHT; i++) {
        gs->board[i] = malloc(gs->map_width * sizeof(char));
    }
    rewind(mapFile);
    x=0;
    y=0;
    logger_out("MAP: map width is %d", gs->map_width);
    while ((buf = fgetc(mapFile)) != EOF) {
        if (buf=='\n') continue;
        fflush(stdout);
        gs->board[x][y] = buf;
        if (buf=='P') {
            logger_out("Found start position at %d,%d",x,y);
            gs->playerx = x;
            gs->playery = y;
        }
        y=(y+1)%gs->map_width;
        if (y==0) x++;
    }
    fclose(mapFile);
}

void init_defaults(struct GameState* gs, int score, int coins, int lives) {
    gs->level = "error";
    gs->playerx = -1;
    gs->playery = -1;
    gs->board = NULL;
    gs->score = score;
    gs->coins = coins;
    gs->lives = lives;
    gs->status = 0x0;
    gs->isSea = 0;
    gs->name = "error";
    gs->playerStatus = 0x0;
    gs->air_time = 0;
    tv_speed.tv_sec = 1;
    tv_delay.tv_usec = 100000;
}

int game_init(struct GameState* gs, char *map, int score, int coins, int lives) {
    char *msg;
    logger_out("INIT: Will start map %s with score %d, coins %d, lives %d", map, score, coins, lives);
    init_defaults(gs, score, coins, lives);
    logger_out("INIT: map structure allocated, parsing level descriptors");
    parse_level_file(gs, map);
    logger_out("INIT: descriptors parsed, parsing map");
    parse_map(gs, map);
}

void game_destroy(struct GameState* gs) {
    int i;
    logger_out("INIT: freeing memory");
    for (i=0; i<SCREEN_HEIGHT; i++) {
        free(gs->board[i]);
    }
    free(gs->board);
    free(gs);
}

void process_arrow(struct GameState* gs, char c) {
    switch (c) {
        case 'A':
            // up
            break;
        case 'B':
            // down
            break;
        case 'C':
            // right
            gs->board[gs->playerx][gs->playery] = '.';
            print_block(gs->playerx, gs->playery, '.');
            gs->playery++;
            gs->board[gs->playerx][gs->playery] = 'P';
            print_block(gs->playerx, gs->playery, 'P');
            break;
        case 'D':
            // left
            if (gs->playery == 0) break;
            gs->board[gs->playerx][gs->playery] = '.';
            print_block(gs->playerx, gs->playery, '.');
            gs->playery--;
            gs->board[gs->playerx][gs->playery] = 'P';
            print_block(gs->playerx, gs->playery, 'P');
            break;
    }
}

int process_input(struct GameState* gs) {
    int read;
    char c;
    fd_set in;
    FD_SET(0, &in);
    read = select(1, &in, NULL, NULL, &tv_speed);   
    if (read) {
        // there's keyboard input
        c = getch();
        if (c==27) {
            read = select(1, &in, NULL, NULL, &tv_delay);
            if (read) {
                c = getch();
                if (c=='[') {
                    // arrow key pressed
                    read = select(1, &in, NULL, NULL, &tv_delay);
                    if (read) {
                        c = getch();
                        process_arrow(gs, c);
                    }
                }
            }
        } else if (c=='q') {
            return 1;
        }
    }
    setformat(7,6,0);
    gotoxy(0,0);
    return 0;
}

void game_loop(struct GameState* gs) {
    int quitting = 0;
    while (!quitting) {
        quitting = process_input(gs);
        
    }
}

#endif