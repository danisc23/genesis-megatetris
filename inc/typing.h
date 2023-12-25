#ifndef __TYPING_H
#define __TYPING_H

#include <genesis.h>

typedef struct
{
    u8 left;
    u8 right;
    u8 up;
    u8 down;
} BoxTile;

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    u8 draw_next_tetromino;
    u8 draw_ghost_tetromino;
    u8 starting_level;
    u8 floor_level;
    u8 shift_grid;
} GameConfig;

enum GAME_STATE
{
    GAME_STATE_MENU,
    GAME_STATE_OPTIONS,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
};

#endif // __TYPING_H