#include "functions.h"
#include "drawing.h"
#include "joy_handlers.h"

static int sign(int x)
{
    return (x > 0) - (x < 0);
}

void joyMainMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    int start_pressed = changed & BUTTON_START;
    int up = sign(changed & BUTTON_UP);
    int down = sign(changed & BUTTON_DOWN);
    int a_pressed = changed & BUTTON_A;
    int b_pressed = changed & BUTTON_B;
    int c_pressed = changed & BUTTON_C;

    int pointer_dir = down - up;
    if (pointer_dir)
        drawMainMenuPointer(updateSelectedOption(pointer_dir));

    triggerSelectedOptionOnCondition(start_pressed || a_pressed || b_pressed || c_pressed);
}

void joyPlaying(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1)
        return;
    int right = sign(changed & state & BUTTON_RIGHT);
    int left = sign(changed & state & BUTTON_LEFT);
    int down = sign(changed & state & BUTTON_DOWN);
    int up = sign(changed & state & BUTTON_UP);
    int start_pressed = changed & state & BUTTON_START;
    int a_pressed = sign(changed & state & BUTTON_A);
    int b_pressed = sign(changed & state & BUTTON_B);
    int c_pressed = sign(changed & state & BUTTON_C);

    int x_dir = right - left;
    int y_dir = down;
    hold_x_dir = x_dir;

    int rotate_clockwise = b_pressed;
    int rotate_counter_clockwise = sign(a_pressed + c_pressed);

    int rotation_dir = rotate_clockwise - rotate_counter_clockwise;

    if (x_dir)
        startMoveSideTimer();

    if (up)
        dropDown();

    rotateTetromino(rotation_dir);
    moveTetromino(x_dir, y_dir, FALSE);

    updateGameStateOnCondition(start_pressed, GAME_STATE_PAUSED);
}

void joyPauseMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    updateGameStateOnCondition(changed & BUTTON_START, GAME_STATE_PLAYING);
}

void joyGameOverMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    updateGameStateOnCondition(changed & BUTTON_START, GAME_STATE_MENU);
}