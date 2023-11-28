#include "typing.h"
#include "functions.h"
#include "drawing.h"

enum GAME_STATE game_state = GAME_STATE_MENU;

// Global Game
int hiscore = 0;

// Current Tetromino
int current_x;
int current_y;
static int current_rotation;
static int current_tetromino_type;
Position current_tetromino[4];

// Next Tetromino
Position next_tetromino[4];
int next_tetromino_type = -1;

// Constant Movement
static int first_hold;
int hold_x_dir;
int hold_y_dir;

// Main Menu
static int selected_option = 0;
static int starting_level = 1;
static int floor_level = 0;
bool draw_next_tetromino = 1;
char options[5][20] = {"Start Game", "Show Next: YES", "Level: 1", "Floor: 0", "Reset Hiscore"};

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

static void clearCompletedLines()
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

static void setTetromino(int tetromino_type, int rotation, Position *tetromino)
{
    for (int i = 0; i < 4; i++)
    {
        tetromino[i] = TETROMINOES[tetromino_type][rotation][i];
    }
}

static void spawnTetromino()
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
    updateGameStateOnCondition(checkCollisionWithSolidTetrominoParts(current_x, current_y), GAME_STATE_GAME_OVER);
}

void fillGrid()
{
    for (int i = GAME_GRID_Y - 1; i >= 0; i--)
    {
        if (i < GAME_GRID_Y - 1 - floor_level)
            break;
        for (int j = 0; j < GAME_GRID_X; j++)
            solid_tetromino_parts[i][j] = random() % 2;
    }
    drawSolidifiedTetrominoParts();
}

void prepareNewGame()
{
    memset(solid_tetromino_parts, 0, sizeof(solid_tetromino_parts));
    score = 0;
    level = starting_level;
    spawnTetromino();
}

void restartMoveDownTimer()
{
    startTimer(DROP_DOWN_TIMER);
    freezed_tick = 0;
}

void moveDown()
{
    int tick = (getTimer(DROP_DOWN_TIMER, 0) + freezed_tick);
    int drop_speed = DROP_SPEED - (level * DROP_LEVEL_MODIFIER);
    drop_speed = hold_y_dir ? min(drop_speed / 2, HOLD_MOVE_SPEED) : drop_speed;
    if (tick < drop_speed)
        return;

    restartMoveDownTimer();
    moveTetromino(0, 1, !hold_y_dir);
}

void dropDown()
{
    int y_dir = 0;
    while (!checkBottomCollision(current_x, y_dir + current_y + 1))
        y_dir++;
    freezed_tick = DROP_SPEED; // ensure solidifyTetromino() is called next frame
    moveTetromino(0, y_dir, TRUE);
}

void startMoveSideTimer(int x_dir)
{
    hold_x_dir = x_dir;
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

static void solidifyTetromino()
{
    XGM_startPlayPCM(SFX_ID_SOLIDIFY, 1, SOUND_PCM_CH3);
    for (int i = 0; i < 4; i++)
    {
        int x = current_x + current_tetromino[i].x;
        int y = current_y + current_tetromino[i].y;
        x -= GAME_AREA.left + 1;
        y -= GAME_AREA.up + 1;
        if (updateGameStateOnCondition(y < 0, GAME_STATE_GAME_OVER))
            break;

        solid_tetromino_parts[y][x] = 1;
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
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);

    if (bottom_collision)
    {
        solidifyTetromino();
        clearCompletedLines();
        drawSolidifiedTetrominoParts();
        spawnTetromino();
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

int updateSelectedOption(int direction)
{
    if (direction)
    {
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
        selected_option += direction;
        selected_option = selected_option < 0 ? 4 : selected_option;
        selected_option = selected_option > 4 ? 0 : selected_option;
    }
    return selected_option;
}

int updateGameStateOnCondition(int change, enum GAME_STATE state)
{
    if (change)
        game_state = state;

    return change;
}

void triggerSelectedOption(int button_pressed, int direction)
{
    XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);

    switch (selected_option)
    {
    case 0:
        updateGameStateOnCondition(button_pressed, GAME_STATE_PLAYING);
        break;
    case 1:
        draw_next_tetromino = !draw_next_tetromino;
        sprintf(options[1], "Show Next: %s", draw_next_tetromino ? "YES" : "NO ");
        drawMainMenu();
        break;
    case 2:
        starting_level = clamp(1, (direction ? direction : button_pressed) + starting_level, 10);
        sprintf(options[2], "Level: %d ", starting_level);
        drawMainMenu();
        break;
    case 3:
        floor_level = clamp(0, (direction ? direction : button_pressed) + floor_level, 10);
        sprintf(options[3], "Floor: %d ", floor_level);
        drawMainMenu();
        break;
    case 4:
        if (!button_pressed)
            break;
        hiscore = 0;
        drawMainMenuFooter();
        break;
    }
}