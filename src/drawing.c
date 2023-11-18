#include "functions.h"
#include "drawing.h"

static void drawMainMenuTitle()
{
    VDP_drawText(TITLE_1, 0, 2);
    VDP_drawText(TITLE_2, 0, 3);
    VDP_drawText(TITLE_3, 0, 4);
    VDP_drawText(TITLE_4, 0, 5);
    VDP_drawText(TITLE_5, 0, 6);
    VDP_drawText(SEPARATOR, 0, 8);
    VDP_drawText(SUBTITLE, 0, 10);
}

static void drawMainMenuOptions()
{
    VDP_drawText(options[0], 14, 17);
    VDP_drawText(options[1], 14, 18);
    VDP_drawText(options[2], 14, 19);
    VDP_drawText(options[3], 14, 20);
}

void drawMainMenuFooter()
{
    char hiscore_text[20];
    sprintf(hiscore_text, "Hi: %d     ", hiscore);
    VDP_drawText(hiscore_text, 1, MAX_Y - 3);

    VDP_drawText("@danisc23", MAX_X - 10, MAX_Y - 3);
}

void drawMainMenuPointer(int direction)
{
    // TODO: looks like a core function (except VDP stuff)
    if (!direction)
        return;

    VDP_clearTextBG(BG_B, 12, 17 + selected_option, 1);
    selected_option += direction;
    selected_option = selected_option < 0 ? 3 : selected_option;
    selected_option = selected_option > 3 ? 0 : selected_option;
    VDP_drawTextBG(BG_B, ">", 12, 17 + selected_option);
}

void drawMainMenu()
{
    drawMainMenuTitle();
    drawMainMenuOptions();
    drawMainMenuPointer(1);
    drawMainMenuFooter();
}

void drawUI()
{
    char score_text[15];
    char level_text[10];
    char hiscore_text[15];
    sprintf(score_text, "Score: %d", score);
    sprintf(hiscore_text, "Hi: %d", hiscore);
    sprintf(level_text, "Level: %d", level);
    VDP_drawText(score_text, 1, 1);
    VDP_drawText(level_text, 1, 2);
    VDP_drawText(hiscore_text, 1, 3);
}

void drawGameArea()
{
    char horizontal[GAME_GRID_X + 2];
    memset(horizontal, '-', GAME_GRID_X + 2);
    VDP_drawText(horizontal, GAME_AREA.left, GAME_AREA.up);
    VDP_drawText(horizontal, GAME_AREA.left, GAME_AREA.down);
    for (int i = GAME_AREA.up + 1; i < GAME_AREA.down; i++)
    {
        VDP_drawText("|", GAME_AREA.left, i);
        VDP_drawText("|", GAME_AREA.right, i);
    }
}

void drawCurrentTetromino()
{
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = current_y + current_tetromino[i].y;
        VDP_drawTextBG(BG_B, "O", x, y);
    }
}

void drawNextTetromino()
{
    // TODO: Looking at the comments below, this should be composed of two static functions
    if (!draw_next_tetromino)
        return;

    // Draw Area Border
    VDP_clearTextArea(GAME_AREA_RIGHT + 2, GAME_AREA_UP + 1, 5, 4);
    char horizontal[6];
    memset(horizontal, '-', 6);
    VDP_drawText(horizontal, GAME_AREA_RIGHT + 2, GAME_AREA_UP);
    VDP_drawText(horizontal, GAME_AREA_RIGHT + 2, GAME_AREA_UP + 6);
    for (int i = GAME_AREA_UP + 1; i < GAME_AREA_UP + 6; i++)
    {
        VDP_drawText("|", GAME_AREA_RIGHT + 2, i);
        VDP_drawText("|", GAME_AREA_RIGHT + 7, i);
    }

    // Draw Next Tetromino
    // TODO: This looks pochito, also should use constants
    int fixed_y = next_tetromino_type == 0 ? 1 : 0;
    int fixed_x = next_tetromino_type == 6 ? -1 : 0;
    int fixed_rotation = next_tetromino_type == 6 ? 3 : 0;
    for (int i = 0; i < 4; i++)
    {
        int x = GAME_AREA_RIGHT + 4 + TETROMINOES[next_tetromino_type][fixed_rotation][i].x + fixed_x;
        int y = GAME_AREA_UP + 2 + TETROMINOES[next_tetromino_type][fixed_rotation][i].y + fixed_y;
        VDP_drawText("O", x, y);
    }
}

void drawSolidifiedTetrominoParts()
{
    clearGameArea();
    for (int y = 0; y < GAME_GRID_Y - 1; y++)
    {
        for (int x = 0; x < GAME_GRID_X; x++)
        {
            if (solid_tetromino_parts[y][x] == 1)
                VDP_drawText("0", x + GAME_AREA.left + 1, y + GAME_AREA.up + 1);
        }
    }
}

void clearGameArea()
{
    VDP_clearTextArea(GAME_AREA.left + 1, GAME_AREA.up + 1, GAME_AREA.right - GAME_AREA.left - 1, GAME_AREA.down - GAME_AREA.up - 1);
}

void clearTetrominoLastPosition()
{
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = current_y + current_tetromino[i].y;
        VDP_clearTextBG(BG_B, x, y, 1);
    }
}