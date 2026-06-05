#include "game.h"

int main(void) {
  GameConfig gameConfig = {.gameTitle = "Game",
                           .cellSize = 128,
                           .screenWidth = 1080,
                           .screenHeight = 720};

  GameRun(&gameConfig);

  return 0;
}

/* vim:set ts=3 sw=2 sts=2 et: */
