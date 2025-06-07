#include "typing.h"
#include "functions.h"
#include "drawing.h"
#include "options.h"
#include "constants.h"
#include "music.h"

#define OPTIONS_Y_OFFSET 12
#define OPTIONS_X_OFFSET 4

static char TEXT_COLOR[3][33] = {
    "[White] Electronika60  Inverted ",
    " White [Electronika60] Inverted ",
    " White  Electronika60 [Inverted]",
};

static char TEXT_MUSIC[3][30] = {
    "[Off] Chao Garden  Tetremix ",
    " Off [Chao Garden] Tetremix ",
    " Off  Chao Garden [Tetremix]",
};

static char TEXT_CONTROLS[2][19] = {
    "[Normal] Reversed ",
    " Normal [Reversed]",
};

SelectedOptions options = {
    .cursor = OPTIONS_COLOR,
    .color = OPTIONS_COLOR_WHITE,
    .music = OPTIONS_MUSIC_CHAO_GARDEN,
    .controls = OPTIONS_CONTROLS_NORMAL,
};

static GameColor options_color_palette[3] = {
    {RGB24_TO_VDPCOLOR(0xFFFFFF), RGB24_TO_VDPCOLOR(0x000000)},
    {RGB24_TO_VDPCOLOR(0x00D200), RGB24_TO_VDPCOLOR(0x001000)},
    {RGB24_TO_VDPCOLOR(0x000000), RGB24_TO_VDPCOLOR(0xFFFFFF)},
};

u8 getCursorYFromOption(s8 option_cursor)
{
    switch (option_cursor)
    {
    case OPTIONS_COLOR:
        return OPTIONS_Y_OFFSET;
    case OPTIONS_MUSIC:
        return OPTIONS_Y_OFFSET + 3;
    case OPTIONS_CONTROLS:
        return OPTIONS_Y_OFFSET + 6;
    case OPTIONS_SAVE_AND_EXIT:
        return OPTIONS_Y_OFFSET + 9;
    case OPTIONS_RESET_GAME_DATA:
        return OPTIONS_Y_OFFSET + 10;
    }
    return 0;
}

static void setMusic()
{
    if (options.music == OPTIONS_MUSIC_OFF)
        XGM_stopPlay();
    else if (options.music == OPTIONS_MUSIC_CHAO_GARDEN)
        XGM_startPlay(game_music);
    else if (options.music == OPTIONS_MUSIC_TETREMIX)
        XGM_startPlay(game_music_alt);
}

static void setColor()
{
    PAL_setColor(0, options_color_palette[(u8)options.color].bg_color);
    PAL_setColor(15, options_color_palette[(u8)options.color].text_color);
}

s8 OPT_getSelectedControls()
{
    return options.controls == OPTIONS_CONTROLS_NORMAL ? 1 : -1;
}

void OPT_drawOptions()
{
    u8 menuOffset = OPTIONS_Y_OFFSET;
    VDP_drawText("Color:", 4, menuOffset++);
    VDP_drawText(TEXT_COLOR[(u8)options.color], 6, menuOffset++);
    menuOffset++;
    VDP_drawText("Music:", 4, menuOffset++);
    VDP_drawText(TEXT_MUSIC[(u8)options.music], 6, menuOffset++);
    menuOffset++;
    VDP_drawText("Controls: (A,B,C)", 4, menuOffset++);
    VDP_drawText(TEXT_CONTROLS[(u8)options.controls], 6, menuOffset++);
    menuOffset++;
    VDP_drawText("Save & Exit", 4, menuOffset++);
    VDP_drawText("Reset Game Data", 4, menuOffset++);
}

void OPT_moveOptionPointer(s8 direction)
{
    u8 last_cursor = getCursorYFromOption(options.cursor);
    options.cursor += direction;
    if (options.cursor > OPTIONS_RESET_GAME_DATA)
        options.cursor = OPTIONS_COLOR;
    else if (options.cursor < OPTIONS_COLOR)
        options.cursor = OPTIONS_RESET_GAME_DATA;
    VDP_clearTextBG(BG_B, OPTIONS_X_OFFSET - 2, last_cursor, 1);
    VDP_drawTextBG(BG_B, ">", OPTIONS_X_OFFSET - 2, getCursorYFromOption(options.cursor));
}

void OPT_triggerOption()
{
    switch (options.cursor)
    {
    case OPTIONS_RESET_GAME_DATA:
        hiscore = 0;
    case OPTIONS_SAVE_AND_EXIT:
        game_state = GAME_STATE_MENU;
        break;
    }
}

void OPT_moveOptionValue(s8 direction)
{
    switch (options.cursor)
    {
    case OPTIONS_COLOR:
        options.color += direction;
        if (options.color > OPTIONS_COLOR_REVERSE)
            options.color = OPTIONS_COLOR_WHITE;
        else if (options.color < OPTIONS_COLOR_WHITE)
            options.color = OPTIONS_COLOR_REVERSE;
        setColor();
        break;
    case OPTIONS_MUSIC:
        options.music += direction;
        if (options.music > OPTIONS_MUSIC_TETREMIX)
            options.music = OPTIONS_MUSIC_OFF;
        else if (options.music < OPTIONS_MUSIC_OFF)
            options.music = OPTIONS_MUSIC_TETREMIX;
        setMusic();
        break;
    case OPTIONS_CONTROLS:
        options.controls += direction;
        if (options.controls > OPTIONS_CONTROLS_REVERSED)
            options.controls = OPTIONS_CONTROLS_NORMAL;
        else if (options.controls < OPTIONS_CONTROLS_NORMAL)
            options.controls = OPTIONS_CONTROLS_REVERSED;
        break;
    }
    OPT_drawOptions();
}

void OPT_loadOptions()
{
    setMusic();
    setColor();
}