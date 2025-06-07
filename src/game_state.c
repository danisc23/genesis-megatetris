#include "functions.h"
#include "drawing.h"
#include "options.h"
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
    if (game_state == GAME_STATE_PLAYING)
        prepareNewGame();
}

void stateGameSettings()
{
    prepareNextState();
    JOY_setEventHandler(joyGameSettingsMenu);
    updateGameSettingsText();
    drawGameSettingsMenu();
    while (game_state == GAME_STATE_GAME_SETTINGS)
        SYS_doVBlankProcess();
    if (game_state == GAME_STATE_PLAYING)
        prepareNewGame();
}

void statePlaying()
{
    prepareNextState();
    JOY_setEventHandler(joyPlaying);
    drawGameArea();
    drawUI();
    drawSolidifiedTetrominoParts(1);
    drawNextTetromino();
    drawPieceCounters();
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

void stateOptions()
{
    prepareNextState();
    JOY_setEventHandler(joyOptions);
    drawMainMenuTitle();
    drawMainMenuFooter();
    OPT_drawOptions();
    OPT_moveOptionPointer(0);
    while (game_state == GAME_STATE_OPTIONS)
        SYS_doVBlankProcess();
    saveGameData();
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

void stateCredits()
{
    prepareNextState();
    JOY_setEventHandler(joyCredits);

    drawMainMenuTitle();
    drawMainMenuFooter();

    VDP_drawText("Developed by Danisc23", 4, 12);
    VDP_drawText("  Portfolio: sc23.site", 4, 13);
    VDP_drawText("  Games: danisc23.itch.io", 4, 14);

    VDP_drawText("Music by JXChip:", 4, 16);
    VDP_drawText("  Sonic Adv 2: Hero Garden (YM2612)", 4, 17);

    VDP_drawText("Created with SGDK", 4, 19);

    VDP_drawText("This game is 100% FREE!", 4, 21);
    VDP_drawText("If you paid for it, let me know!", 4, 22);

    while (game_state == GAME_STATE_CREDITS)
        SYS_doVBlankProcess();
}