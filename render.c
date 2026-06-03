#include "render.h"
#include "game.h"
#include "raylib.h"

void RenderGrid(GameData *gameData, int cellSize) {
  for (int y = 0; y < GAME_ROW; y++) {
    for (int x = 0; x < GAME_COLUMN; x++) {
      int posX = (x * cellSize);
      int posY = (y * cellSize);

      Rectangle r = (Rectangle){posX, posY, cellSize, cellSize};

      DrawRectangleLinesEx(r, 4, GRAY);
    }
  }
}

void RenderUpdate(GameData *gameData, int cellSize) {
  float mousePosX = gameData->mouseWorldPosition.x;
  float mousePosY = gameData->mouseWorldPosition.y;

  BeginDrawing();
  ClearBackground(SKYBLUE);
  BeginMode2D(gameData->camera);

  RenderGrid(gameData, cellSize);
  DrawColumn(gameData, cellSize);

  DrawCircle(mousePosX, mousePosY, 8, RED);
  EndMode2D();

  DrawText(TextFormat("Mouse World Position: %.2f, %.2f",
                      gameData->mouseWorldPosition.x,
                      gameData->mouseWorldPosition.y),
           4, 4, 40, RAYWHITE);

  EndDrawing();
}

void DrawColumn(GameData *gameData, int cellSize) {
  // Vector2 mousePos = gameData->mousePosition;
  int screenHeight = 1080;

  if (gameData->mouseWorldPosition.x > 0 &&
      gameData->mouseWorldPosition.x < cellSize * GAME_COLUMN) {
    DrawRectangle(((int)(gameData->mouseWorldPosition.x / cellSize) * cellSize),
                  -gameData->camera.offset.y, cellSize, screenHeight,
                  Fade(GRAY, 0.5));
  }
}

// int draw_puck(RenderConfig *renderConfig, int column, int row, Color color) {
//   int radius = renderConfig->puckRadius;
//   int cellSize = renderConfig->cellSize;
//   int x = column * cellSize;
//   int y = row * cellSize;
//
//   DrawCircle(x, y, radius, color);
// }

/* vim:set ts=3 sw=2 sts=2 et: */
