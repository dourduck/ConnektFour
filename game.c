#include "game.h"
#include "raylib.h"
#include "render.h"

void InputUpdate(GameData *gameData) {
  gameData->mouseWorldPosition =
      GetScreenToWorld2D(GetMousePosition(), gameData->camera);

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }
}


void Start(GameData *gameData, GameConfig *config) {
  SetConfigFlags(FLAG_FULLSCREEN_MODE);

  int screenWidth = config->screenWidth;
  int screenHeight = config->screenHeight;
  const char *title = config->gameTitle;
  int cellSize = config->cellSize;

  InitWindow(screenWidth, screenHeight, title);
  SetExitKey(KEY_Q);

  gameData->camera.zoom = 1.0;
  gameData->camera.target = (Vector2){0, 0};
  gameData->camera.offset =
      (Vector2){(int)(1920 / 2) - (int)(cellSize * GAME_COLUMN / 2),
                (int)(1080 / 2) - (int)(cellSize * GAME_ROW / 2)};

  gameData->camera.rotation = 0;
}

void GameUpdate(GameData *gameData) {
    // float dt = GetFrameTime();
    InputUpdate(gameData);
}

void StartUpdate(GameData *gameData, GameConfig *gameConfig) {
  Start(gameData, gameConfig);
  int cellSize = gameConfig->cellSize;

  while (!WindowShouldClose()) {
    GameUpdate(gameData);
    RenderUpdate(gameData, cellSize);
  }

  CloseWindow();
}

/* vim:set ts=3 sw=2 sts=2 et: */
