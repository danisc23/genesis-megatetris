#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <genesis.h>

enum CURSOR_ENUM
{
    OPTIONS_COLOR,
    OPTIONS_MUSIC,
    OPTIONS_CONTROLS,
    OPTIONS_SAVE_AND_EXIT,
    OPTIONS_RESET_GAME_DATA
};

enum COLOR_ENUM
{
    OPTIONS_COLOR_WHITE,
    OPTIONS_COLOR_ELEKTRONIKA60,
    OPTIONS_COLOR_REVERSE
};

enum MUSIC_ENUM
{
    OPTIONS_MUSIC_OFF,
    OPTIONS_MUSIC_CHAO_GARDEN,
    OPTIONS_MUSIC_TETREMIX,
};

enum CONTROLS_ENUM
{
    OPTIONS_CONTROLS_NORMAL,
    OPTIONS_CONTROLS_REVERSED
};

typedef struct
{
    u16 text_color;
    u16 bg_color;
} GameColor;

typedef struct
{
    s8 cursor;
    s8 color;
    s8 music;
    s8 controls;
} SelectedOptions;

extern SelectedOptions options;

s8 OPT_getSelectedControls();
void OPT_loadOptions();
void OPT_drawOptions();
void OPT_triggerOption();
void OPT_moveOptionValue(s8 direction);
void OPT_moveOptionPointer(s8 direction);

#endif /* __OPTIONS_H */
