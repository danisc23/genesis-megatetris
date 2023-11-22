#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "typing.h"
#include "constants.h"

// Global Game
extern enum GAME_STATE game_state;
extern int hiscore;

// Current Tetromino
extern int current_x;
extern int current_y;
extern int current_rotation;
extern int current_tetromino_type;
extern Position current_tetromino[4];

// Next Tetromino
extern Position next_tetromino[4];
extern int next_tetromino_type;

// Horizontal Constant Movement
extern int hold_x_dir;
extern int first_hold;

// Main Menu
extern char options[4][20];
extern bool draw_next_tetromino;

// Current Game
extern int freezed_tick; // Used to track elapsed time when game is paused
extern int score;
extern int level;
extern int solid_tetromino_parts[GAME_GRID_Y][GAME_GRID_X];

// Game Logic
void prepareNewGame();
void moveDown();
void dropDown();
void startMoveSideTimer();
void moveSide();
void spawnTetromino();
void setTetromino(int tetromino_type, int, Position *tetromino);
void moveTetromino(int x, int y, bool silent);
void rotateTetromino(int direction);
void solidifyTetromino();

void clearCompletedLines();

int updateGameStateOnCondition(int condition, enum GAME_STATE);
int updateSelectedOption(int direction);
void triggerSelectedOptionOnCondition(int condition);

#endif // __FUNCTIONS_H