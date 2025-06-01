#include "functions.h"
#include "drawing.h"

static int last_drawn_pointer = OPTIONS_Y_OFFSET;

// Used to reduce flickering on redraws
static int drawn_solid_tetromino_parts[GAME_GRID_Y][GAME_GRID_X] = {0};

void resetMenuPointer()
{
    last_drawn_pointer = OPTIONS_Y_OFFSET;
}

void drawMainMenuTitle()
{
    VDP_drawText(TITLE_1, 0, 1);
    VDP_drawText(TITLE_2, 0, 2);
    VDP_drawText(TITLE_3, 0, 3);
    VDP_drawText(TITLE_4, 0, 4);
    VDP_drawText(TITLE_5, 0, 5);
    VDP_drawText(SEPARATOR, 0, 7);
    VDP_drawText(SUBTITLE, 0, 9);
}

static void drawMainMenuOptions()
{
    int max_options = sizeof(menu_options) / sizeof(menu_options[0]);
    for (u8 i = 0; i < max_options; i++)
        VDP_drawText(menu_options[i], 14, OPTIONS_Y_OFFSET + i);
}

void drawMainMenuFooter()
{
    char hiscore_text[40];
    sprintf(hiscore_text, "Hi: %ld            ", hiscore);
    VDP_drawText(hiscore_text, 1, MAX_Y - 2);

    VDP_drawText(GAME_VERSION, MAX_X - 19, MAX_Y - 2);
}

void drawMainMenuPointer(int selected_option)
{
    VDP_clearTextBG(BG_B, 12, last_drawn_pointer, 1);
    last_drawn_pointer = OPTIONS_Y_OFFSET + selected_option;
    VDP_drawTextBG(BG_B, ">", 12, last_drawn_pointer);
}

void drawMainMenu()
{
    drawMainMenuTitle();
    drawMainMenuOptions();
    drawMainMenuPointer(selected_option);
    drawMainMenuFooter();
}

static void drawGameSettingOptions()
{
    int max_options = sizeof(game_setting_options) / sizeof(game_setting_options[0]);
    for (u8 i = 0; i < max_options; i++)
        VDP_drawText(game_setting_options[i], 12, OPTIONS_Y_OFFSET + i);
}

void drawGameSettingsMenuPointer(int selected_option)
{
    VDP_clearTextBG(BG_B, 10, last_drawn_pointer, 1);
    last_drawn_pointer = OPTIONS_Y_OFFSET + selected_option;
    VDP_drawTextBG(BG_B, ">", 10, last_drawn_pointer);
}

void drawGameSettingsMenu()
{
    drawMainMenuTitle();
    drawGameSettingOptions();
    drawGameSettingsMenuPointer(selected_game_setting);
    drawMainMenuFooter();
}

void drawUI()
{
    char score_text[20];
    char level_text[15];
    char lines_text[20];
    sprintf(score_text, "Score: %ld", score);
    sprintf(level_text, "Level: %d", level);
    sprintf(lines_text, "Lines: %d", total_lines_cleared);
    VDP_drawText(score_text, 1, 1);
    VDP_drawText(level_text, 1, 2);
    VDP_drawText(lines_text, 1, 3);
    drawMainMenuFooter();
}

void drawGameArea()
{
    char horizontal[GAME_GRID_X + 3];
    memset(horizontal, '-', GAME_GRID_X + 2);
    horizontal[GAME_GRID_X + 2] = '\0';
    VDP_drawText(horizontal, GAME_AREA.left, GAME_AREA.up);
    VDP_drawText(horizontal, GAME_AREA.left, GAME_AREA.down);
    for (int i = GAME_AREA.up + 1; i < GAME_AREA.down; i++)
    {
        VDP_drawText("|", GAME_AREA.left, i);
        VDP_drawText("|", GAME_AREA.right, i);
    }
}

static Position last_ghost_tetromino[4] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};

static void drawGhostTetromino()
{
    int ghost_y = 1;
    while (!checkBottomCollision(current_x, ghost_y))
    {
        ghost_y++;
    }
    ghost_y--;
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = ghost_y + current_tetromino[i].y;
        VDP_clearTextBG(BG_B, last_ghost_tetromino[i].x, last_ghost_tetromino[i].y, 1);
        last_ghost_tetromino[i].x = x;
        last_ghost_tetromino[i].y = y;
        VDP_drawTextBG(BG_B, ".", x, y);
    }
}

void drawCurrentTetromino()
{
    if (game_config.draw_ghost_tetromino)
        drawGhostTetromino();
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = current_y + current_tetromino[i].y;
        VDP_drawTextBG(BG_B, "O", x, y);
    };
}

static void drawNextTetrominoArea()
{
    char horizontal[6] = "-----";
    VDP_clearTextArea(GAME_AREA_RIGHT + 1, GAME_AREA_UP + 1, 5, 4);
    VDP_drawText(horizontal, GAME_AREA_RIGHT + 1, GAME_AREA_UP);
    VDP_drawText(horizontal, GAME_AREA_RIGHT + 1, GAME_AREA_UP + 6);
    for (int i = GAME_AREA_UP + 1; i < GAME_AREA_UP + 6; i++)
    {
        VDP_drawText("|", GAME_AREA_RIGHT + 5, i);
    }
}

static void drawNextTetrominoContent()
{
    // TODO: This looks bad, also should use constants
    int fixed_y = next_tetromino_type == 0 ? 1 : 0;
    int fixed_x = next_tetromino_type == 6 ? -1 : 0;
    int fixed_rotation = next_tetromino_type == 6 ? 3 : 0;
    for (int i = 0; i < 4; i++)
    {
        u16 x = GAME_AREA_RIGHT + 2 + TETROMINOES[next_tetromino_type][fixed_rotation][i].x + fixed_x;
        u16 y = GAME_AREA_UP + 2 + TETROMINOES[next_tetromino_type][fixed_rotation][i].y + fixed_y;
        VDP_drawText("O", x, y);
    }
}

void drawNextTetromino()
{
    if (!game_config.draw_next_tetromino)
        return;

    drawNextTetrominoArea();
    drawNextTetrominoContent();
}

void drawSolidifiedTetrominoParts(int force_redraw)
{
    if (force_redraw)
        memset(drawn_solid_tetromino_parts, 0, sizeof(drawn_solid_tetromino_parts));

    for (u16 y = 0; y < GAME_GRID_Y - 1; y++)
    {
        for (u16 x = 0; x < GAME_GRID_X; x++)
        {
            if (solid_tetromino_parts[y][x] == 1 && drawn_solid_tetromino_parts[y][x] != 1)
            {
                VDP_drawText("0", x + GAME_AREA.left + 1, y + GAME_AREA.up + 1);
                drawn_solid_tetromino_parts[y][x] = 1;
            }
            else if (solid_tetromino_parts[y][x] == 0 && drawn_solid_tetromino_parts[y][x] == 1)
            {
                drawn_solid_tetromino_parts[y][x] = 0;
                VDP_clearText(x + GAME_AREA.left + 1, y + GAME_AREA.up + 1, 1);
            }
        }
    }
}

void clearGameScreen()
{
    VDP_clearTextArea(0, 0, MAX_X, MAX_Y);
    VDP_clearTextAreaBG(BG_B, 0, 0, MAX_X, MAX_Y);
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