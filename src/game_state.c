#include "functions.h"
#include "drawing.h"
#include "joy_handlers.h"
#include "game_state.h"

static void prepareNextState()
{
    JOY_setEventHandler(NULL);
    // TODO: looks like a draw function
    VDP_clearTextArea(0, 0, MAX_X, MAX_Y);
    VDP_clearTextAreaBG(BG_B, 0, 0, MAX_X, MAX_Y);
}

void stateMenu()
{
    prepareNextState();
    JOY_setEventHandler(joyMainMenu);
    selected_option = -1; // TODO: fast fix, check this
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
    // TODO: freezed_tick should be updated in a function and not be a global variable
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