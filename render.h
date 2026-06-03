#ifndef RENDER_H
#define RENDER_H

#include "game.h"
// #include "raylib.h"

typedef struct {
  int puckRadius;
  int cellSize;
} RenderConfig;

void RenderGrid(GameData *gameData, int cellSize);
void RenderUpdate(GameData *gameData, int cellSize);
void DrawColumn(GameData *gameData, int cellSize);
// void AnimatePuck(float dt);
// int draw_puck(RenderConfig* renderConfig, int column, int row, Color color);

#endif
/* vim:set ts=3 sw=2 sts=2 et: */
