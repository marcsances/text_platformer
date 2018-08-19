#include <stdio.h>
#include <stdlib.h>
#include "cutils.h"
#include "renderer.c"
#include "state.c"
#include "logger.c"

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
}

void parse_level_file(struct GameState* gs, char *map) {
    char *levelFileName, *key, *value, *msg;
    FILE *levelFile;

    levelFileName = malloc(strlen(map) + 12);
    sprintf(levelFileName, "levels/%s.lev", map);
    msg = malloc(strlen("LEVEL: loading level file ") + strlen(levelFileName));
    sprintf(msg,"LEVEL: loading level file %s",levelFileName);
    logger_out(msg);
    levelFile = fopen(levelFileName, "r");
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
        key = malloc(64);
        value = malloc(64);
    }
    fclose(levelFile);
}

int get_width_for_filesize(int size) {
    return (size/25) - 1;
}

void parse_map(struct GameState* gs, char *map) {
    char *levelFileName, buf, *msg;
    FILE* mapFile;
    int size, mapWidth, i, x, y;
    levelFileName = malloc(strlen(map) + 12);
    sprintf(levelFileName, "levels/%s.map", map);
    msg = malloc(strlen("MAP: loading map file ") + strlen(levelFileName));
    sprintf(msg,"MAP: loading map file %s",levelFileName);
    logger_out(msg);
    mapFile = fopen(levelFileName, "r");
    fseek(mapFile, 0L, SEEK_END);
    size = ftell(mapFile);
    mapWidth = get_width_for_filesize(size);
    gs->board = malloc(25 * sizeof(char*));
    for (i=0; i<25; i++) {
        gs->board[i] = malloc(mapWidth * sizeof(char));
    }
    rewind(mapFile);
    x=0;
    y=0;
    msg = malloc(strlen("MAP: map width is 99999999"));
    sprintf(msg,"MAP: map width is %d", mapWidth);
    logger_out(msg);
    while (buf = fgetc(mapFile) != EOF) {
        printf("store %d,%d,%c",x,y,buf);
        fflush(stdout);
        gs->board[y][x] = buf;
        if (buf=='P') {
            msg = malloc(strlen("Found start position at 9999,9999"));
            sprintf(msg,"Found start position at %d,%d",x,y);
            logger_out(msg);            
            gs->playerx = x;
            gs->playery = y;
        }
        x=(x+1)%mapWidth;
        if (x==0) y++;
    }
    fclose(mapFile);
}

int game_init(struct GameState* gs, char *map, int score, int coins, int lives) {
    char *msg;
    msg = malloc(strlen("INIT: Will start map %s with score 99999999, coins 9999, lives 99" + strlen(map)));
    sprintf(msg,"INIT: Will start map %s with score %d, coins %d, lives %d", map, score, coins, lives);
    logger_out(msg);
    gs = malloc(sizeof(struct GameState));
    init_defaults(gs, score, coins, lives);
    logger_out("INIT: map structure allocated, parsing level descriptors");
    parse_level_file(gs, map);
    logger_out("INIT: descriptors parsed, parsing map");
    parse_map(gs, map);
}


int main(int argc, char *argv[]) {
    struct GameState* gs;
    logger_init("stdout.log", "stderr.log", 1);
    logger_out("INIT: initialise game");
    game_init(gs, "level1", 0, 0, 3);
    logger_out("INIT: closing");
    logger_destroy();
    return 0;
}