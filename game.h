#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define GAME_COLUMN 7
#define GAME_ROW 7

typedef enum {
  PLAYER_BLUE,
  PLAYER_RED,
} Player;

typedef enum {
  EMPTY,
  CELL_BLUE,
  CELL_RED,
} Cell;

typedef struct {
  Cell cells[GAME_ROW][GAME_COLUMN];
} Grid;

typedef struct {
  const char *gameTitle;
  int cellSize;
  int screenWidth;
  int screenHeight;
} GameConfig;

typedef struct {
  Grid grid;
  Player currentPlayer;
  Vector2 mouseWorldPosition;
  Camera2D camera;
} GameData;

void InputUpdate(GameData *gameData);
void GameUpdate(GameData *gameData);
void Start(GameData *gameData, GameConfig *config);
void StartUpdate(GameData *gameData, GameConfig *gameConfig);

#endif

/* vim:set ts=3 sw=2 sts=2 et: */
