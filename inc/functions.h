#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "typing.h"
#include "constants.h"

// Global Game
extern enum GAME_STATE game_state;
extern u32 hiscore;

// Current Tetromino
extern int current_x;
extern int current_y;
extern Position current_tetromino[4];

// Next Tetromino
extern Position next_tetromino[4];
extern int next_tetromino_type;

// Constant Movement
extern int hold_x_dir;
extern int hold_y_dir;

// Main Menu
extern char menu_options[6][22];
extern GameConfig game_config;

// Current Game
extern int freezed_tick; // Used to track elapsed time when game is paused
extern u32 score;
extern u16 total_lines_cleared;
extern s8 lines_for_next_level;
extern u8 level;
extern int solid_tetromino_parts[GAME_GRID_Y][GAME_GRID_X];

void loadGameData();
void saveGameData();

// Game Logic
void prepareNewGame();
void restartMoveDownTimer();
void moveDown();
void dropDown();
void startMoveSideTimer(int x_dir);
void moveSide();
void moveTetromino(int x, int y, bool silent);
void rotateTetromino(int direction);
bool checkBottomCollision(int pos_x, int pos_y);

int updateGameStateOnCondition(int condition, enum GAME_STATE);
int updateSelectedOption(int direction);
void triggerSelectedOption(int button_pressed, int direction);

#endif // __FUNCTIONS_H