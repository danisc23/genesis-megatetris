#include "functions.h"
#include "game_state.h"
#include "music.h"
#include "sfx.h"

static void initSound()
{
  XGM_startPlay(game_music);
  XGM_setPCM(SFX_ID_CLEAR_LINE, line_clear_sfx, sizeof(line_clear_sfx));
  XGM_setPCM(SFX_ID_SOLIDIFY, solidify_sfx, sizeof(solidify_sfx));
  XGM_setPCM(SFX_ID_MOVE, move_sfx, sizeof(move_sfx));
}

int main()
{
  JOY_init();
  VDP_setScreenWidth320();
  initSound();

  while (1)
  {
    switch (game_state)
    {
    case GAME_STATE_MENU:
      stateMenu();
      break;
    case GAME_STATE_PLAYING:
      statePlaying();
      break;
    case GAME_STATE_PAUSED:
      statePause();
      break;
    case GAME_STATE_GAME_OVER:
      stateGameOver();
      break;
    }
  }
  return (0);
}