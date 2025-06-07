#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define GAME_VERSION "v1.0.0 - 2023-2025"

// Tetromino Set constants
#define TETROMINO_SET_ALL 0
#define TETROMINO_SET_I 1
#define TETROMINO_SET_O 2
#define TETROMINO_SET_L 3
#define TETROMINO_SET_J 4
#define TETROMINO_SET_S 5
#define TETROMINO_SET_Z 6
#define TETROMINO_SET_T 7

#define ALGORITHM_RANDOM 0
#define ALGORITHM_FAIR 1

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

#define SCORE_1_LINE 40
#define SCORE_2_LINES 100
#define SCORE_3_LINES 300
#define SCORE_TETRIS 1200
#define SCORE_SOFT_DROP 1

#define DROP_DOWN_TIMER 1
#define TICKS_PER_FRAME SUBTICKPERSECOND / 60
#define MIN_FRAMES_PER_ROW 3

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
