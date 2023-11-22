#ifndef __DRAWING_H
#define __DRAWING_H

#define OPTIONS_Y_OFFSET 17

void drawMainMenuPointer(int selected_option);
void drawMainMenu();
void drawMainMenuFooter();
void drawUI();
void drawGameArea();
void drawCurrentTetromino();
void drawNextTetromino();
void drawSolidifiedTetrominoParts();
void clearGameScreen();
void clearGameArea();
void clearTetrominoLastPosition();

#endif // __DRAWING_H