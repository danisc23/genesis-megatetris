#include "functions.h"
#include "drawing.h"
#include "joy_handlers.h"
#include "game_state.h"

static void prepareNextState()
{
    JOY_setEventHandler(NULL);
    clearGameScreen();
}

void stateMenu()
{
    prepareNextState();
    JOY_setEventHandler(joyMainMenu);
    drawMainMenu();
    while (game_state == GAME_STATE_MENU)
    {
        SYS_doVBlankProcess();
    }
    prepareNewGame();
}

void statePlaying()
{
    prepareNextState();
    JOY_setEventHandler(joyPlaying);
    drawGameArea();
    drawUI();
    drawSolidifiedTetrominoParts();
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
    freezed_tick += getTimer(DROP_DOWN_TIMER, 0);
    while (game_state == GAME_STATE_PAUSED)
    {
        SYS_doVBlankProcess();
    }
}

void stateGameOver()
{
    prepareNextState();
    JOY_setEventHandler(joyGameOverMenu);
    hiscore = score > hiscore ? score : hiscore;
    drawUI();
    VDP_drawText("GAME OVER", 15, 10);
    VDP_drawText("PRESS START", 14, 20);
    while (game_state == GAME_STATE_GAME_OVER)
    {
        SYS_doVBlankProcess();
    }
}