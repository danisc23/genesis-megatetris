#include "typing.h"
#include "functions.h"
#include "drawing.h"

enum GAME_STATE game_state = GAME_STATE_MENU;

// Global Game
int hiscore = 0;

// Current Tetromino
int current_x;
int current_y;
int current_rotation;
int current_tetromino_type;
Position current_tetromino[4];

// Next Tetromino
Position next_tetromino[4];
int next_tetromino_type = -1;

// Horizontal Constant Movement
int hold_x_dir;
int first_hold;

// Main Menu
static int selected_option = 0;
char options[4][20] = {"Start Game", "Show Next: NO", "Level: 1", "Reset Hi-Score"};
bool draw_next_tetromino = 0;
int starting_level = 1;

// Current Game
int freezed_tick = 0; // Used to track elapsed time when game is paused
int score = 0;
int level = 1;
int solid_tetromino_parts[GAME_GRID_Y][GAME_GRID_X];

static bool checkCollisionWithSolidTetrominoParts(int pos_x, int pos_y)
{
    for (int i = 0; i < 4; i++)
    {
        int seek_y = pos_y + current_tetromino[i].y - GAME_AREA.up - 1;
        if (seek_y < 0)
            continue;
        int seek_x = pos_x + current_tetromino[i].x - GAME_AREA.left - 1;

        if (solid_tetromino_parts[seek_y][seek_x] == 1)
        {
            return TRUE;
        }
    }
    return FALSE;
}

static bool checkBottomCollision(int pos_x, int pos_y)
{
    for (int i = 0; i < 4; i++)
    {
        int y = pos_y + current_tetromino[i].y;
        if (y == GAME_AREA.down)
        {
            return TRUE;
        }
    }
    return checkCollisionWithSolidTetrominoParts(pos_x, pos_y);
}

static bool checkWallCollision(int new_x, int new_y)
{
    for (int i = 0; i < 4; i++)
    {
        int x = new_x + current_tetromino[i].x;
        if (x == GAME_AREA.left || x == GAME_AREA.right)
        {
            return TRUE;
        }
    }
    return checkCollisionWithSolidTetrominoParts(new_x, new_y);
}

void prepareNewGame()
{
    memset(solid_tetromino_parts, 0, sizeof(solid_tetromino_parts));
    score = 0;
    level = starting_level;
    spawnTetromino();
}

void moveDown()
{
    int tick = getTimer(DROP_DOWN_TIMER, 0) + freezed_tick;
    if (tick < DROP_SPEED - (level * DROP_LEVEL_MODIFIER))
        return;

    freezed_tick = 0;
    moveTetromino(0, 1, TRUE);
    startTimer(DROP_DOWN_TIMER);
}

void dropDown()
{
    int y_dir = 0;
    while (!checkBottomCollision(current_x, y_dir + current_y + 1))
    {
        y_dir++;
    }
    freezed_tick = DROP_SPEED; // ensure solidifyTetromino() is called next frame
    moveTetromino(0, y_dir, TRUE);
}

void startMoveSideTimer()
{
    startTimer(HOLD_TIMER);
    first_hold = TRUE;
}

void moveSide()
{
    int tick = getTimer(HOLD_TIMER, 0);
    int speed = first_hold ? INITIAL_HOLD_MOVE_SPEED : HOLD_MOVE_SPEED;
    if (tick < speed)
        return;

    first_hold = FALSE;
    moveTetromino(hold_x_dir, 0, FALSE);
    startTimer(HOLD_TIMER);
}

void spawnTetromino()
{
    current_rotation = 0;
    if (next_tetromino_type == -1)
    {
        setRandomSeed(getTick());
        next_tetromino_type = random() % 7;
    }
    current_tetromino_type = next_tetromino_type;
    next_tetromino_type = random() % 7;
    setTetromino(current_tetromino_type, current_rotation, current_tetromino);
    setTetromino(next_tetromino_type, 0, next_tetromino);
    current_x = GAME_AREA_LEFT + (GAME_GRID_X / 2);
    current_y = GAME_AREA_UP + 1;
    drawNextTetromino();
}

void setTetromino(int tetromino_type, int rotation, Position *tetromino)
{
    for (int i = 0; i < 4; i++)
    {
        tetromino[i] = TETROMINOES[tetromino_type][rotation][i];
    }
}

void moveTetromino(int x, int y, bool silent)
{
    if (!x && !y)
        return;
    clearTetrominoLastPosition();
    int new_x = current_x + x;
    int new_y = current_y + y;

    bool wall_collision = checkWallCollision(new_x, new_y);
    bool bottom_collision = checkBottomCollision(current_x, new_y);

    current_x = wall_collision ? current_x : new_x;
    current_y = bottom_collision ? current_y : new_y;

    if (!silent && !wall_collision && !bottom_collision)
    {
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
    }

    if (bottom_collision)
    {
        solidifyTetromino();
        clearCompletedLines();
        spawnTetromino();
        drawSolidifiedTetrominoParts();
    }
}

void rotateTetromino(int direction)
{
    if (!direction)
        return;
    clearTetrominoLastPosition();
    current_rotation = (current_rotation + direction);
    if (current_rotation < 0)
        current_rotation = 3;
    else if (current_rotation > 3)
        current_rotation = 0;
    setTetromino(current_tetromino_type, current_rotation, current_tetromino);

    for (int i = 0; i <= 4; i++)
    {
        current_x -= i * (i % 2 == 0 ? -1 : 1);
        if (!checkWallCollision(current_x, current_y))
        {
            XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
            return;
        }
    }

    current_x -= 2;
    current_rotation = (current_rotation + (2 * direction)) % 4;
    rotateTetromino(direction);
}

void solidifyTetromino()
{
    XGM_startPlayPCM(SFX_ID_SOLIDIFY, 1, SOUND_PCM_CH3);
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = current_y + current_tetromino[i].y;
        x -= GAME_AREA.left + 1;
        y -= GAME_AREA.up + 1;
        if (updateGameStateOnCondition(y <= 0, GAME_STATE_GAME_OVER))
            break;

        solid_tetromino_parts[y][x] = 1;
    }
}

void clearCompletedLines()
{
    int new_tetromino_parts[GAME_GRID_Y][GAME_GRID_X];
    memset(new_tetromino_parts, 0, sizeof(new_tetromino_parts));
    int next_new_y = GAME_GRID_Y - 1;
    int lines_cleared = 0;

    for (int y = GAME_GRID_Y - 1; y >= 0; y--)
    {
        bool completed_line = TRUE;
        for (int x = 0; x < GAME_GRID_X; x++)
        {
            if (solid_tetromino_parts[y][x] == 0)
            {
                memcpy(new_tetromino_parts[next_new_y], solid_tetromino_parts[y], sizeof(solid_tetromino_parts[y]));
                next_new_y--;
                completed_line = FALSE;
                break;
            }
        }
        if (completed_line)
            lines_cleared++;
    }
    memcpy(solid_tetromino_parts, new_tetromino_parts, sizeof(solid_tetromino_parts));
    if (!lines_cleared)
        return;

    XGM_startPlayPCM(SFX_ID_CLEAR_LINE, 1, SOUND_PCM_CH2);
    score += (lines_cleared * lines_cleared) * 100;
    level = score / 1000 + starting_level;
    level = level > 10 ? 10 : level;
    drawUI();
}

int updateSelectedOption(int direction)
{
    if (direction)
    {
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
        selected_option += direction;
        selected_option = selected_option < 0 ? 3 : selected_option;
        selected_option = selected_option > 3 ? 0 : selected_option;
    }
    return selected_option;
}

int updateGameStateOnCondition(int change, enum GAME_STATE state)
{
    if (change)
        game_state = state;

    return change;
}

void triggerSelectedOptionOnCondition(int condition)
{
    // TODO: This changes game variables and draws to the screen
    // that belong to different domains (game and drawing) :thinking:
    if (!condition)
        return;

    XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);

    if (selected_option == 0)
        updateGameStateOnCondition(condition, GAME_STATE_PLAYING);
    else if (selected_option == 1)
    {
        draw_next_tetromino = !draw_next_tetromino;
        sprintf(options[1], "Show Next: %s", draw_next_tetromino ? "YES" : "NO ");
        VDP_drawText(options[1], 14, 18);
    }
    else if (selected_option == 2)
    {
        starting_level++;
        starting_level = starting_level > 10 ? 1 : starting_level;
        sprintf(options[2], "Level: %d ", starting_level);
        VDP_drawText(options[2], 14, 19);
    }
    else if (selected_option == 3)
    {
        hiscore = 0;
        drawMainMenuFooter();
    }
}