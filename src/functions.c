#include "typing.h"
#include "functions.h"
#include "drawing.h"
#include "options.h"

enum GAME_STATE game_state = GAME_STATE_MENU;

// Global Game
u32 hiscore = 0;

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

GameConfig game_config = {
    .draw_next_tetromino = 1,
    .draw_ghost_tetromino = 0,
    .starting_level = 1,
    .floor_level = 0,
    .algorithm = ALGORITHM_RANDOM,
    .tetromino_set = TETROMINO_SET_ALL,
    .show_piece_counter = 0,
    .shift_grid = 0};

// Main Menu
int selected_option = 0;
char menu_options[4][22] = {
    "Start Game",
    "Game Settings",
    "Options",
    "Credits"};

// Game Settings Menu
int selected_game_setting = 0;
char game_setting_options[9][22] = {
    "Level : 1 ",
    "Lines : 0 ",
    "See Next : YES ",
    "Ghost Hint : NO ",
    "Algorithm : RANDOM ",
    "Spawn : ALL ",
    "Shape Count : NO ",
    "Shift Grid : NO ",
    "Save & Exit"};

// Current Game
int freezed_tick = 0; // Used to track elapsed time when game is paused
u32 score = 0;
u16 total_lines_cleared = 0;
s8 lines_for_next_level = 0;
u8 level = 1;
int solid_tetromino_parts[GAME_GRID_Y][GAME_GRID_X] = {0};

// Piece counter arrays
int piece_counters[7] = {0};

static u8 SRAM_HIGHSCORE_OFFSET = 0x00;
static u8 SRAM_OPTIONS_OFFSET = 0x05;
static u8 SRAM_GAME_CONFIG_OFFSET = 0x08;
static u8 SRAM_CHECKSUM_OFFSET = 0x33;
static u8 SRAM_CHECKSUM_VALUE = 69;

static void initSRAM()
{
    SRAM_enableRO();
    u8 test_value = SRAM_readByte(SRAM_CHECKSUM_OFFSET);
    SRAM_disable();
    if (test_value != SRAM_CHECKSUM_VALUE)
    {
        u8 optionsOffset = SRAM_OPTIONS_OFFSET;
        u8 configOffset = SRAM_GAME_CONFIG_OFFSET;
        SRAM_enable();
        SRAM_writeByte(SRAM_CHECKSUM_OFFSET, SRAM_CHECKSUM_VALUE);
        SRAM_writeLong(SRAM_HIGHSCORE_OFFSET, 0);

        // Initialize options
        SRAM_writeByte(optionsOffset++, options.color);
        SRAM_writeByte(optionsOffset++, options.music);
        SRAM_writeByte(optionsOffset++, options.controls); // Initialize game config settings
        SRAM_writeByte(configOffset++, game_config.starting_level);
        SRAM_writeByte(configOffset++, game_config.floor_level);
        SRAM_writeByte(configOffset++, game_config.draw_next_tetromino);
        SRAM_writeByte(configOffset++, game_config.draw_ghost_tetromino);
        SRAM_writeByte(configOffset++, game_config.algorithm);
        SRAM_writeByte(configOffset++, game_config.tetromino_set);
        SRAM_writeByte(configOffset++, game_config.show_piece_counter);
        SRAM_writeByte(configOffset++, game_config.shift_grid);

        SRAM_disable();
    }
}

void saveGameData()
{
    SRAM_enable();
    SRAM_writeLong(SRAM_HIGHSCORE_OFFSET, hiscore);
    u8 optionsOffset = SRAM_OPTIONS_OFFSET;
    SRAM_writeByte(optionsOffset++, options.color);
    SRAM_writeByte(optionsOffset++, options.music);
    SRAM_writeByte(optionsOffset++, options.controls); // Save game config settings
    u8 configOffset = SRAM_GAME_CONFIG_OFFSET;
    SRAM_writeByte(configOffset++, game_config.starting_level);
    SRAM_writeByte(configOffset++, game_config.floor_level);
    SRAM_writeByte(configOffset++, game_config.draw_next_tetromino);
    SRAM_writeByte(configOffset++, game_config.draw_ghost_tetromino);
    SRAM_writeByte(configOffset++, game_config.algorithm);
    SRAM_writeByte(configOffset++, game_config.tetromino_set);
    SRAM_writeByte(configOffset++, game_config.show_piece_counter);
    SRAM_writeByte(configOffset++, game_config.shift_grid);

    SRAM_disable();
}

void loadGameData()
{
    initSRAM();
    SRAM_enableRO();
    hiscore = SRAM_readLong(SRAM_HIGHSCORE_OFFSET);
    u8 optionsOffset = SRAM_OPTIONS_OFFSET;
    options.color = SRAM_readByte(optionsOffset++);
    options.music = SRAM_readByte(optionsOffset++);
    options.controls = SRAM_readByte(optionsOffset++); // Load game config settings
    u8 configOffset = SRAM_GAME_CONFIG_OFFSET;
    game_config.starting_level = SRAM_readByte(configOffset++);
    game_config.floor_level = SRAM_readByte(configOffset++);
    game_config.draw_next_tetromino = SRAM_readByte(configOffset++);
    game_config.draw_ghost_tetromino = SRAM_readByte(configOffset++);
    game_config.algorithm = SRAM_readByte(configOffset++);
    game_config.tetromino_set = SRAM_readByte(configOffset++);
    game_config.show_piece_counter = SRAM_readByte(configOffset++);
    game_config.shift_grid = SRAM_readByte(configOffset++);

    // Ensure valid values (in case it's first run or corrupt data)
    if (game_config.starting_level < 1 || game_config.starting_level > 10)
        game_config.starting_level = 1;
    if (game_config.tetromino_set > TETROMINO_SET_T)
        game_config.tetromino_set = TETROMINO_SET_ALL;

    SRAM_disable();
}

static void addScore(u16 points)
{
    score += points;
    if (score > hiscore)
        hiscore = score;
    drawUI();
}

static void addLinesCleared(u16 lines)
{
    total_lines_cleared += lines;
    lines_for_next_level -= lines;
    if (lines_for_next_level <= 0)
    {
        level++;
        lines_for_next_level += level >= 10 ? 20 : 10;
    }
}

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

bool checkBottomCollision(int pos_x, int pos_y)
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

static u16 getScoreByCompletedLines(int lines_cleared)
{
    switch (lines_cleared)
    {
    case 1:
        return SCORE_1_LINE;
    case 2:
        return SCORE_2_LINES;
    case 3:
        return SCORE_3_LINES;
    case 4:
        return SCORE_TETRIS;
    default:
        return 0;
    }
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
    addLinesCleared(lines_cleared);
    addScore(getScoreByCompletedLines(lines_cleared) * level);
}

static void setTetromino(int tetromino_type, int rotation, Position *tetromino)
{
    for (int i = 0; i < 4; i++)
    {
        tetromino[i] = TETROMINOES[tetromino_type][rotation][i];
    }
}

static int getNextTetrominoType()
{
    if (game_config.tetromino_set == TETROMINO_SET_ALL)
    {
        if (game_config.algorithm == ALGORITHM_FAIR)
        {
            // Find the minimum count among all pieces
            int min_count = piece_counters[0];
            for (int i = 1; i < 7; i++)
            {
                if (piece_counters[i] < min_count)
                {
                    min_count = piece_counters[i];
                }
            }

            // Create array of all pieces that have the minimum count
            int least_used_pieces[7];
            int least_used_count = 0;

            for (int i = 0; i < 7; i++)
            {
                if (piece_counters[i] == min_count)
                {
                    least_used_pieces[least_used_count] = i;
                    least_used_count++;
                }
            }

            // Randomly select one of the least used pieces
            return least_used_pieces[random() % least_used_count];
        }
        else
        {
            // ALGORITHM_RANDOM
            return random() % 7;
        }
    }
    else
    {
        // Tetromino type is (tetromino_set - 1) for specific pieces
        return game_config.tetromino_set - 1;
    }
}

static void spawnTetromino()
{
    current_rotation = 0;
    if (next_tetromino_type == -1)
    {
        setRandomSeed(getTick());
        next_tetromino_type = getNextTetrominoType();
    }
    current_tetromino_type = next_tetromino_type;
    next_tetromino_type = getNextTetrominoType();
    setTetromino(current_tetromino_type, current_rotation, current_tetromino);
    setTetromino(next_tetromino_type, 0, next_tetromino);
    current_x = GAME_AREA_LEFT + (GAME_GRID_X / 2);
    current_y = GAME_AREA_UP + 1;
    drawNextTetromino();
    updateGameStateOnCondition(checkCollisionWithSolidTetrominoParts(current_x, current_y), GAME_STATE_GAME_OVER);
}

static void fillGrid()
{
    for (int i = GAME_GRID_Y - 1; i >= 0; i--)
    {
        if (i < GAME_GRID_Y - 1 - game_config.floor_level)
            break;
        for (int j = 0; j < GAME_GRID_X; j++)
            solid_tetromino_parts[i][j] = random() % 2;
    }
    drawSolidifiedTetrominoParts(0);
}

void prepareNewGame()
{
    memset(solid_tetromino_parts, 0, sizeof(solid_tetromino_parts));
    score = 0;
    level = game_config.starting_level - 1;
    total_lines_cleared = 0;
    lines_for_next_level = 0;
    addLinesCleared(0);
    fillGrid();
    // Reset piece counters for new game
    resetPieceCounters();
    // Reset next_tetromino_type to force a new piece based on current settings
    next_tetromino_type = -1;
    spawnTetromino();
    drawPieceCounters();
}

void restartMoveDownTimer()
{
    startTimer(DROP_DOWN_TIMER);
    freezed_tick = 0;
}

static u8 getFramesPerRow()
{
    // GB~ Speed curve
    if (level <= 5)
        return 56 - (level * 4);
    else if (level <= 9)
        return 28 - ((level - 6) * 5);
    else
        return max(10 - ((level - 10) * 0.7), MIN_FRAMES_PER_ROW);
}

void moveDown()
{
    u32 tick = (getTimer(DROP_DOWN_TIMER, 0) + freezed_tick);
    u32 drop_speed = TICKS_PER_FRAME * getFramesPerRow();
    drop_speed = hold_y_dir ? min(drop_speed / 2, HOLD_MOVE_SPEED) : drop_speed;
    if (tick < drop_speed)
        return;

    if (hold_y_dir)
        addScore(SCORE_SOFT_DROP);

    restartMoveDownTimer();
    moveTetromino(0, 1, !hold_y_dir);
}

void dropDown()
{
    int y_dir = 0;
    while (!checkBottomCollision(current_x, y_dir + current_y + 1))
        y_dir++;
    freezed_tick = SUBTICKPERSECOND; // ensure solidifyTetromino() is called next frame
    addScore(y_dir);
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

static void shiftSolidifiedTetrominoParts()
{
    // Move the grid elements one position to the right, the overflowing elements will be moved to first position
    for (int i = 0; i < GAME_GRID_Y; i++)
    {
        int temp = solid_tetromino_parts[i][GAME_GRID_X - 1];
        for (int j = GAME_GRID_X - 1; j > 0; j--)
            solid_tetromino_parts[i][j] = solid_tetromino_parts[i][j - 1];
        solid_tetromino_parts[i][0] = temp;
    }
}

static void solidifyTetromino()
{
    XGM_startPlayPCM(SFX_ID_SOLIDIFY, 1, SOUND_PCM_CH3);

    // Increment piece counter for the current tetromino type
    piece_counters[current_tetromino_type]++;
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

    // Apply shift grid if enabled
    if (game_config.shift_grid)
    {
        shiftSolidifiedTetrominoParts();
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
        drawSolidifiedTetrominoParts(0);
        spawnTetromino();
        drawPieceCounters();
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
        int max_option = sizeof(menu_options) / sizeof(menu_options[0]) - 1;
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
        selected_option += direction;
        selected_option = selected_option < 0 ? max_option : selected_option;
        selected_option = selected_option > max_option ? 0 : selected_option;
    }
    return selected_option;
}

int updateSelectedGameSetting(int direction)
{
    if (direction)
    {
        int max_option = sizeof(game_setting_options) / sizeof(game_setting_options[0]) - 1;
        XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
        selected_game_setting += direction;
        selected_game_setting = selected_game_setting < 0 ? max_option : selected_game_setting;
        selected_game_setting = selected_game_setting > max_option ? 0 : selected_game_setting;
    }
    return selected_game_setting;
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
        if (!button_pressed)
            break;
        updateGameStateOnCondition(TRUE, GAME_STATE_GAME_SETTINGS);
        break;
    case 2:
        if (!button_pressed)
            break;
        updateGameStateOnCondition(TRUE, GAME_STATE_OPTIONS);
        break;
    case 3:
        if (!button_pressed)
            break;
        updateGameStateOnCondition(TRUE, GAME_STATE_CREDITS);
        break;
    }
}

void triggerSelectedGameSetting(int button_pressed, int direction)
{
    XGM_startPlayPCM(SFX_ID_MOVE, 1, SOUND_PCM_CH2);
    switch (selected_game_setting)
    {
    case 0:
        game_config.starting_level = clamp(1, (direction ? direction : button_pressed) + game_config.starting_level, 10);
        sprintf(game_setting_options[0], "Level: %d ", game_config.starting_level);
        drawGameSettingsMenu();
        break;
    case 1:
        game_config.floor_level = clamp(0, (direction ? direction : button_pressed) + game_config.floor_level, 10);
        sprintf(game_setting_options[1], "Lines: %d ", game_config.floor_level);
        drawGameSettingsMenu();
        break;
    case 2:
        game_config.draw_next_tetromino = !game_config.draw_next_tetromino;
        sprintf(game_setting_options[2], "See Next: %s", game_config.draw_next_tetromino ? "YES" : "NO ");
        drawGameSettingsMenu();
        break;
    case 3:
        game_config.draw_ghost_tetromino = !game_config.draw_ghost_tetromino;
        sprintf(game_setting_options[3], "Ghost Hint: %s", game_config.draw_ghost_tetromino ? "YES" : "NO ");
        drawGameSettingsMenu();
        break;
    case 4:
        game_config.algorithm = !game_config.algorithm;
        char *algorithm_name = (game_config.algorithm == ALGORITHM_FAIR) ? "FAIR  " : "RANDOM";
        sprintf(game_setting_options[4], "Algorithm: %s", algorithm_name);
        drawGameSettingsMenu();
        break;
    case 5:
        if (direction || button_pressed)
        {
            // Handle direction or button press for cycling through shapes
            int change = direction ? direction : (button_pressed ? 1 : 0);

            // Since tetromino_set is unsigned (u8), we need to handle the wrapping differently
            if (change < 0 && game_config.tetromino_set == 0)
                game_config.tetromino_set = 7;
            else
                game_config.tetromino_set = (game_config.tetromino_set + change) % 8;
        }

        char *shape_name;
        switch (game_config.tetromino_set)
        {
        case TETROMINO_SET_ALL:
            shape_name = "ALL";
            break;
        case TETROMINO_SET_I:
            shape_name = "I  ";
            break;
        case TETROMINO_SET_O:
            shape_name = "O  ";
            break;
        case TETROMINO_SET_L:
            shape_name = "L  ";
            break;
        case TETROMINO_SET_J:
            shape_name = "J  ";
            break;
        case TETROMINO_SET_S:
            shape_name = "S  ";
            break;
        case TETROMINO_SET_Z:
            shape_name = "Z  ";
            break;
        case TETROMINO_SET_T:
            shape_name = "T  ";
            break;
        default:
            shape_name = "ALL";
        }
        sprintf(game_setting_options[5], "Spawn: %s", shape_name);
        drawGameSettingsMenu();
        break;
    case 6:
        game_config.show_piece_counter = !game_config.show_piece_counter;
        sprintf(game_setting_options[6], "Shape Count: %s", game_config.show_piece_counter ? "YES" : "NO ");
        drawGameSettingsMenu();
        break;
    case 7:
        game_config.shift_grid = !game_config.shift_grid;
        sprintf(game_setting_options[7], "Shift Grid: %s", game_config.shift_grid ? "YES" : "NO ");
        drawGameSettingsMenu();
        break;
    case 8:
        if (button_pressed)
        {
            saveGameData();
            selected_option = 0; // Reset main menu cursor
            resetMenuPointer();  // Reset cursor position
            game_state = GAME_STATE_MENU;
        }
        break;
    }
}

void updateGameSettingsText()
{
    sprintf(game_setting_options[0], "Level: %d ", game_config.starting_level);
    sprintf(game_setting_options[1], "Lines: %d ", game_config.floor_level);
    sprintf(game_setting_options[2], "See Next: %s", game_config.draw_next_tetromino ? "YES" : "NO ");
    sprintf(game_setting_options[3], "Ghost Hint: %s", game_config.draw_ghost_tetromino ? "YES" : "NO ");

    char *algorithm_name = (game_config.algorithm == ALGORITHM_FAIR) ? "FAIR  " : "RANDOM";
    sprintf(game_setting_options[4], "Algorithm: %s", algorithm_name);

    char *shape_name;
    switch (game_config.tetromino_set)
    {
    case TETROMINO_SET_ALL:
        shape_name = "ALL";
        break;
    case TETROMINO_SET_I:
        shape_name = "I  ";
        break;
    case TETROMINO_SET_O:
        shape_name = "O  ";
        break;
    case TETROMINO_SET_L:
        shape_name = "L  ";
        break;
    case TETROMINO_SET_J:
        shape_name = "J  ";
        break;
    case TETROMINO_SET_S:
        shape_name = "S  ";
        break;
    case TETROMINO_SET_Z:
        shape_name = "Z  ";
        break;
    case TETROMINO_SET_T:
        shape_name = "T  ";
        break;
    default:
        shape_name = "ALL";
    }
    sprintf(game_setting_options[5], "Spawn: %s", shape_name);
    sprintf(game_setting_options[6], "Shape Count: %s", game_config.show_piece_counter ? "YES" : "NO ");
    sprintf(game_setting_options[7], "Shift Grid: %s", game_config.shift_grid ? "YES" : "NO ");
}

// Piece counter functions
void resetPieceCounters()
{
    for (int i = 0; i < 7; i++)
    {
        piece_counters[i] = 0;
    }
}