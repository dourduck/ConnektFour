#include "render.h"
#include "game.h"
#include "raylib.h"

/* vvv DEBUG CONSLE vvv */
void Draw_DebugConsole(Input *input) {
  Rectangle r = {.x = 0, .y = 0, .width = GetRenderWidth(), .height = 300};
  DrawRectangleRec(r, Fade(BLACK, 0.6));

  int x = (input->mouseWorldPositionQuantized.x);
  int y = (input->mouseWorldPositionQuantized.y);

  DrawText(TextFormat("Mouse World Position Quantized: (%d, %d)", x, y), 10, 10,
           24, RAYWHITE);
}
/* ^^^ DEBUG CONSLE ^^^ */

void RenderUpdate(World *world, Input *input, Camera2D camera, int cellSize,
                  int lineWidth, int puckRadius) {
  BeginDrawing();
  ClearBackground(SKYBLUE);
  BeginMode2D(camera);

  for (int i = 1; i <= world->nextID; i++) {
    TraitFlag flag = TRAITS_PUCK | TRAITS_POSITIONABLE;

    if (world->active[i] && WorldEntity_HasTrait(world, i, flag)) {
      Puck puck = world->puck[i];

      float x = world->x[i];
      float y = world->y[i];

      if (puck == PUCK_BLUE) {
        DrawCircle(x, y, puckRadius, BLUE);
      } else if (puck == PUCK_RED) {
        DrawCircle(x, y, puckRadius, RED);
      } else {
        DrawCircle(x, y, puckRadius, MAGENTA);
      }
    }
  }

  for (int i = 1; i <= world->nextID; i++) {
    TraitFlag flag = TRAITS_CELL | TRAITS_POSITIONABLE;

    if (world->active[i] && WorldEntity_HasTrait(world, i, flag)) {
      float x = world->x[i];
      float y = world->y[i];
      Rectangle r = {.x = x, .y = y, .width = cellSize, .height = cellSize};
      DrawRectangleLinesEx(r, lineWidth, DARKGRAY);
    }
  }

  int x = (input->mouseWorldPositionQuantized.x);
  int y = (input->mouseWorldPositionQuantized.y);

  if (world->console) {
    if (x >= 0 && x < (GAME_COLUMN * cellSize) && y >= 0 &&
        y < (GAME_ROW * cellSize)) {
      DrawCircle(x + (cellSize / 2), y + (cellSize / 2), 8, MAGENTA);
    }
  }

  EndMode2D();

  if (world->console) {
    Draw_DebugConsole(input);
  }

  EndDrawing();
}

// /* vim:set ts=3 sw=2 sts=2 et: */
