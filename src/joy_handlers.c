#include "functions.h"
#include "drawing.h"
#include "joy_handlers.h"
#include "options.h"

static int sign(int x)
{
    return (x > 0) - (x < 0);
}

void joyMainMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    int start_pressed = state & changed & BUTTON_START;
    int up = sign(state & changed & BUTTON_UP);
    int down = sign(state & changed & BUTTON_DOWN);
    int a_pressed = state & changed & BUTTON_A;
    int b_pressed = state & changed & BUTTON_B;
    int c_pressed = state & changed & BUTTON_C;
    int left_pressed = state & changed & BUTTON_LEFT;
    int right_pressed = state & changed & BUTTON_RIGHT;

    int direction = sign(right_pressed || a_pressed || c_pressed) - sign(left_pressed || b_pressed);
    int button = sign(start_pressed || a_pressed || b_pressed || c_pressed);
    int pointer_dir = down - up;

    if (pointer_dir)
        drawMainMenuPointer(updateSelectedOption(pointer_dir));
    else if (direction || button)
        triggerSelectedOption(button, direction);
}

void joyOptions(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    int start_pressed = state & changed & BUTTON_START;
    int up = sign(state & changed & BUTTON_UP);
    int down = sign(state & changed & BUTTON_DOWN);
    int a_pressed = state & changed & BUTTON_A;
    int b_pressed = state & changed & BUTTON_B;
    int c_pressed = state & changed & BUTTON_C;
    int left_pressed = state & changed & BUTTON_LEFT;
    int right_pressed = state & changed & BUTTON_RIGHT;

    int direction = sign(right_pressed) - sign(left_pressed);
    int input = sign(start_pressed || a_pressed || b_pressed);
    int quick_exit = sign(c_pressed);
    int pointer_dir = down - up;

    if (pointer_dir)
        OPT_moveOptionPointer(pointer_dir);
    else if (direction)
        OPT_moveOptionValue(direction);
    else if (input)
        OPT_triggerOption();
    else if (quick_exit)
    {
        saveGameData();
        game_state = GAME_STATE_MENU;
    }
}

void joyPlaying(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1)
        return;
    int right = sign(changed & state & BUTTON_RIGHT);
    int left = sign(changed & state & BUTTON_LEFT);
    int down = sign(changed & state & BUTTON_DOWN);
    int left_released = (changed & BUTTON_LEFT) && !left;
    int right_released = (changed & BUTTON_RIGHT) && !right;
    int down_released = sign((changed & BUTTON_DOWN) && !down);
    int up = sign(changed & state & BUTTON_UP);
    int start_pressed = changed & state & BUTTON_START;
    int a_pressed = sign(changed & state & BUTTON_A);
    int b_pressed = sign(changed & state & BUTTON_B);
    int c_pressed = sign(changed & state & BUTTON_C);

    int x_dir = right - left;
    int y_dir = down;

    int rotate_clockwise = b_pressed;
    int rotate_counter_clockwise = sign(a_pressed + c_pressed);

    int rotation_dir = rotate_clockwise - rotate_counter_clockwise;

    if (x_dir || right_released || left_released)
        startMoveSideTimer(x_dir ? x_dir : 0);

    hold_y_dir = down ? 1 : down_released ? 0
                                          : hold_y_dir;
    if (down || down_released)
        restartMoveDownTimer();
    else if (up)
        dropDown();

    rotateTetromino(rotation_dir * (options.controls ? -1 : 1));
    moveTetromino(x_dir, y_dir, FALSE);

    updateGameStateOnCondition(start_pressed, GAME_STATE_PAUSED);
}

void joyPauseMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    updateGameStateOnCondition(state & changed & BUTTON_START, GAME_STATE_PLAYING);
}

void joyGameOverMenu(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;
    updateGameStateOnCondition(state & changed & BUTTON_START, GAME_STATE_MENU);
}

void joyCredits(u16 joy, u16 changed, u16 state)
{
    if (joy != JOY_1 || !state)
        return;

    // Any button press returns to main menu
    int any_button = state & changed & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START | BUTTON_X | BUTTON_Y | BUTTON_Z);
    updateGameStateOnCondition(any_button, GAME_STATE_MENU);
}