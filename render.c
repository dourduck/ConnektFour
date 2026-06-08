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
  ClearBackground(DARKGRAY);
  BeginMode2D(camera);

  for (int i = 1; i <= world->nextID; i++) {
    TraitFlag flag = TRAITS_PUCK | TRAITS_POSITIONABLE;

    if (world->active[i] && WorldEntity_HasTrait(world, i, flag)) {
      Puck puck = world->puck[i];

      float x = world->x[i];
      float y = world->y[i];

      if (puck == PUCK_BLUE) {
        DrawCircle(x, y, puckRadius, SKYBLUE);
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
      DrawRectangleLinesEx(r, lineWidth, LIGHTGRAY);
    }
  }

  int x = (input->mouseWorldPositionQuantized.x);
  int y = (input->mouseWorldPositionQuantized.y);

  if (world->console) {
    for (int i = 0; i < GAME_COLUMN * GAME_ROW; i++) {
      int xx = ((i % GAME_COLUMN) * cellSize) + cellSize / 2;
      int yy = ((i / GAME_COLUMN) * cellSize) + cellSize / 2;
      Puck puck = world->grid[i];
      Color color = puck == PUCK_BLUE  ? DARKBLUE
                    : puck == PUCK_NIL ? LIGHTGRAY
                                       : MAROON;
      DrawCircle(xx, yy, 8, color);
    }

    if (x >= 0 && x < (GAME_COLUMN * cellSize) && y >= 0 &&
        y < (GAME_ROW * cellSize)) {
      DrawCircle(x + (cellSize / 2), y + (cellSize / 2), 8, MAGENTA);
    }
  }

  EndMode2D();

  if (world->console) {
    Draw_DebugConsole(input);
  }

  if (world->winner == PUCK_BLUE) {
    Rectangle r = {
        .x = (int)((GetRenderWidth() / 2) -
                   (MeasureText("Press R to restart...", 100) / 2) - 100),
        .y = (((MeasureTextEx(
                    GetFontDefault(),
                    "BLUE WON!!!\nPress R to restart...\nPress Q to quit", 100,
                    1.0))
                   .y) /
              2) -
             75,
        .width = MeasureText("Press R to restart...", 100) + 200,
        .height =
            MeasureTextEx(GetFontDefault(),
                          "BLUE WON!!!\nPress R to restart...\nPress Q to quit",
                          100, 1.0)
                .y};
    DrawRectangleRec(r, Fade(BLACK, 0.6));
    DrawText("BLUE WON!!!", 500, 100, 100, SKYBLUE);
    DrawText("Press R to restart...", 500, 200, 100, PURPLE);
    DrawText("Press Q to quit", 500, 300, 50, DARKBLUE);
  } else if (world->winner == PUCK_RED) {
    Rectangle r = {
        .x = (int)((GetRenderWidth() / 2) -
                   (MeasureText("Press R to restart...", 100) / 2) - 100),
        .y = (((MeasureTextEx(
                    GetFontDefault(),
                    "BLUE WON!!!\nPress R to restart...\nPress Q to quit", 100,
                    1.0))
                   .y) /
              2) -
             75,
        .width = MeasureText("Press R to restart...", 100) + 200,
        .height =
            MeasureTextEx(GetFontDefault(),
                          "RED WON!!!\nPress R to restart...\nPress Q to quit",
                          100, 1.0)
                .y};
    DrawRectangleRec(r, Fade(BLACK, 0.6));
    DrawText("RED WON!!!", 500, 100, 100, PINK);
    DrawText("Press R to restart...", 500, 200, 100, PURPLE);
    DrawText("Press Q to quit", 500, 300, 50, DARKBLUE);
  }

  EndDrawing();
}

// /* vim:set ts=3 sw=2 sts=2 et: */
