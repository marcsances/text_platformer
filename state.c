#ifndef STATE_C
#define STATE_C


enum GameStatus { 
    LOADING=0x0, PRE_LEVEL=0x1, IN_LEVEL=0x2, POST_LEVEL=0x3
};

enum PlayerStatus {
    GROUND=0x0, JUMPING=0x1, FALLING=0x2, END_GAME=0x3
};

struct GameState {
    // game status info
    enum GameStatus status;

    // map info
    char* level;
    char* name;
    char* nextLevel;
    int map_width;
    int isSea;

    // player info
    int playerx;
    int playery;
    int score;
    int coins;
    int lives;
    enum PlayerStatus playerStatus;
    int air_time;

    // map environment
    char **board;
};

#endif