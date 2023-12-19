#include "functions.h"
#include "drawing.h"
#include "joy_handlers.h"
#include "game_state.h"

static void prepareNextState()
{
    // Clean hold_x_dir and hold_y_dir
    hold_x_dir = 0;
    hold_y_dir = 0;

    JOY_setEventHandler(NULL);
    clearGameScreen();
}

void stateMenu()
{
    prepareNextState();
    JOY_setEventHandler(joyMainMenu);
    drawMainMenu();
    while (game_state == GAME_STATE_MENU)
        SYS_doVBlankProcess();
}

void statePlaying()
{
    prepareNewGame();
    prepareNextState();
    JOY_setEventHandler(joyPlaying);
    drawGameArea();
    drawUI();
    drawSolidifiedTetrominoParts(1);
    drawNextTetromino();
    startTimer(DROP_DOWN_TIMER);
    while (game_state == GAME_STATE_PLAYING)
    {
        moveDown();
        moveSide();
        drawCurrentTetromino();
        SYS_doVBlankProcess();
    }
}

void statePause()
{
    prepareNextState();
    JOY_setEventHandler(joyPauseMenu);
    drawUI();
    drawGameArea();
    VDP_drawText("PAUSE", 18, 13);
    // TODO: freezed_tick should be updated in a function and not be a global variable
    freezed_tick += getTimer(DROP_DOWN_TIMER, 0);
    while (game_state == GAME_STATE_PAUSED)
        SYS_doVBlankProcess();
}

void stateGameOver()
{
    JOY_setEventHandler(joyGameOverMenu);
    if (score > hiscore)
    {
        hiscore = score;
        drawUI();
    }
    saveGameData();
    VDP_drawText("          ", 16, 12);
    VDP_drawText("GAME  OVER", 16, 13);
    VDP_drawText("          ", 16, 14);
    while (game_state == GAME_STATE_GAME_OVER)
        SYS_doVBlankProcess();
}