#include "game.h"

int main(void) {
  GameConfig gameConfig;
  gameConfig.gameTitle = "game";
  gameConfig.screenWidth = 1080;
  gameConfig.screenHeight = 720;
  gameConfig.cellSize = 128;

  GameData gameData;

  StartUpdate(&gameData, &gameConfig);

  return 0;
}

/* vim:set ts=3 sw=2 sts=2 et: */
