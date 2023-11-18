#include "functions.h"
#include "game_state.h"
#include "music.h"
#include "sfx.h"

int main()
{
  JOY_init();
  VDP_setScreenWidth320();
  XGM_startPlay(game_music);
  XGM_setPCM(SFX_ID_CLEAR_LINE, line_clear_sfx, sizeof(line_clear_sfx));
  XGM_setPCM(SFX_ID_SOLIDIFY, solidify_sfx, sizeof(solidify_sfx));
  XGM_setPCM(SFX_ID_MOVE, move_sfx, sizeof(move_sfx));

  while (1)
  {
    switch (game_state)
    {
    case 0: // TODO: Use game_state enum
      stateMenu();
      break;
    case 1:
      statePlaying();
      break;
    case 2:
      statePause();
      break;
    case 3:
      stateGameOver();
      break;
    }
  }
  return (0);
}