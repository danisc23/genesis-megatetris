#include "functions.h"
#include "game_state.h"
#include "options.h"
#include "sfx.h"

static void initSound()
{
  XGM_setPCM(SFX_ID_CLEAR_LINE, line_clear_sfx, sizeof(line_clear_sfx));
  XGM_setPCM(SFX_ID_SOLIDIFY, solidify_sfx, sizeof(solidify_sfx));
  XGM_setPCM(SFX_ID_MOVE, move_sfx, sizeof(move_sfx));
}

int main()
{
  loadGameData();
  OPT_loadOptions();
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
    case GAME_STATE_GAME_SETTINGS:
      stateGameSettings();
      break;
    case GAME_STATE_OPTIONS:
      stateOptions();
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
    case GAME_STATE_CREDITS:
      stateCredits();
      break;
    }
  }
  return (0);
}