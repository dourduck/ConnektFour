#include "render.h"
#include "game.h"
#include "raylib.h"

void RenderUpdate(World *world, Camera2D camera, int cellSize, int lineWidth,
                  int puckRadius) {
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

  EndMode2D();

  EndDrawing();
}

// /* vim:set ts=3 sw=2 sts=2 et: */
