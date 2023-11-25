#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define GAME_VERSION "v0.2.0 - 2023"

#define SFX_ID_CLEAR_LINE 64
#define SFX_ID_SOLIDIFY 65
#define SFX_ID_MOVE 66

#define PIECE_SIZE 8
#define MAX_X 320 / PIECE_SIZE
#define MAX_Y 224 / PIECE_SIZE
#define GAME_GRID_X 10
#define GAME_GRID_Y 18
#define SCREEN_CENTER_X MAX_X / 2
#define SCREEN_CENTER_Y MAX_Y / 2
#define GAME_AREA_LEFT SCREEN_CENTER_X - GAME_GRID_X / 2
#define GAME_AREA_RIGHT 1 + SCREEN_CENTER_X + GAME_GRID_X / 2
#define GAME_AREA_UP SCREEN_CENTER_Y - GAME_GRID_Y / 2
#define GAME_AREA_DOWN SCREEN_CENTER_Y + GAME_GRID_Y / 2
#define DROP_DOWN_TIMER 1
#define DROP_SPEED 110000
#define DROP_LEVEL_MODIFIER 10000
#define HOLD_TIMER 2
#define INITIAL_HOLD_MOVE_SPEED 20000
#define HOLD_MOVE_SPEED 10000

#define TITLE_1 "  ____ _____ _   _ _____ ____ ___ ____  "
#define TITLE_2 " / ___| ____| \\ | | ____/ ___|_ _/ ___| "
#define TITLE_3 "| |  _|  _| |  \\| |  _| \\___ \\| |\\___ \\ "
#define TITLE_4 "| |_| | |___| |\\  | |___ ___) | | ___) |"
#define TITLE_5 " \\____|_____|_| \\_|_____|____|___|____/ "
#define SEPARATOR "________________________________________"
#define SUBTITLE "      M  E  G  A  T  E  T  R  I  S"

extern BoxTile GAME_AREA;
extern Position TETROMINOES[7][4][4];

#endif // __CONSTANTS_H
