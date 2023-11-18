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

enum GAME_STATE
{
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
};

#endif // __TYPING_H