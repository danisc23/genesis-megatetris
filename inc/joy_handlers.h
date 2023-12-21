#ifndef __JOY_HANDLERS_H
#define __JOY_HANDLERS_H

#include <genesis.h>

void joyMainMenu(u16 joy, u16 changed, u16 state);
void joyOptions(u16 joy, u16 changed, u16 state);
void joyPlaying(u16 joy, u16 changed, u16 state);
void joyPauseMenu(u16 joy, u16 changed, u16 state);
void joyGameOverMenu(u16 joy, u16 changed, u16 state);

#endif // __JOY_HANDLERS_H