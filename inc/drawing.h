#ifndef __DRAWING_H
#define __DRAWING_H

#define OPTIONS_Y_OFFSET 16
#define GAME_SETTINGS_Y_OFFSET 14

void resetMenuPointer();
void drawMainMenuTitle();
void drawMainMenuPointer(int selected_option);
void drawMainMenu();
void drawMainMenuFooter();
void drawGameSettingsMenuPointer(int selected_option);
void drawGameSettingsMenu();
void drawUI();
void drawGameArea();
void drawCurrentTetromino();
void drawNextTetromino();
void drawSolidifiedTetrominoParts(int force_redraw);
void drawPieceCounters();
void clearGameScreen();
void clearTetrominoLastPosition();

#endif // __DRAWING_H