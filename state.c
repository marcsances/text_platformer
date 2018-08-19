#ifndef STATE_C
#define STATE_C
#endif

enum GameStatus { 
    LOADING=0x0, PRE_LEVEL=0x1, IN_LEVEL=0x2, POST_LEVEL=0x3
};

struct GameState {
    char* level;
    int playerx;
    int playery;
    char **board;
    int score;
    int coins;
    int lives;
    enum GameStatus status;
    char* nextLevel;
    int isSea;
    char* name;
};

