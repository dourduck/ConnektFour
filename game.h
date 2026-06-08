#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

#define GRAVITY 800
#define MAX_ENTITIES 128
#define NIL 0

#define GAME_COLUMN 7
#define GAME_ROW 7

typedef struct {
  const char *gameTitle;
  int cellSize;
  int screenWidth;
  int screenHeight;
} GameConfig;

typedef int EntityID;

typedef uint_fast8_t Traits;

typedef float VelocityX;
typedef float VelocityY;

typedef float PositionX;
typedef float PositionY;

typedef char *Text;
typedef enum {
  TRAITS_NIL = NIL,
  TRAITS_POSITIONABLE = (1 << 0),
  TRAITS_PHYSICS = (1 << 1),
  TRAITS_TEXT = (1 << 2),
  TRAITS_PUCK = (1 << 3),
  TRAITS_CELL = (1 << 4),
} TraitFlag;

typedef enum { PUCK_NIL = NIL, PUCK_BLUE = 1, PUCK_RED = 2 } Puck;

typedef struct World World;

struct World {
  EntityID nextID;

  bool active[MAX_ENTITIES];

  Traits traits[MAX_ENTITIES];

  PositionX x[MAX_ENTITIES];
  PositionY y[MAX_ENTITIES];

  VelocityX dx[MAX_ENTITIES];
  VelocityY dy[MAX_ENTITIES];

  Text text[MAX_ENTITIES];
  Puck puck[MAX_ENTITIES];

  bool console;
  bool puckFalling;

  int columnStopPosition[7];

  int puckColumnIndex[MAX_ENTITIES];
  int currentPuckIndex;
  Puck currentPuckTeam;
  Puck grid[GAME_COLUMN * GAME_ROW];

  Puck winner;
};

typedef struct {
  Vector2 mouseWorldPosition;
  Vector2 mouseWorldPositionQuantized;
  bool keyPressed_F;
  bool keyPressed_GRAVE;
  bool mouseLeftPressed;

} Input;

World World_Create();

EntityID WorldEntity_Create(World *world);

void WorldEntity_TraitsAdd(World *world, EntityID entityID, TraitFlag flag);

void WorldEntity_TraitsRemove(World *world, EntityID entityID, TraitFlag flag);

void WorldEntity_TraitsClear(World *world, EntityID entityID);

bool WorldEntity_HasTrait(World *world, EntityID entityID, TraitFlag flag);

void WorldEntity_Remove(World *world, EntityID entityID);

void WorldEntity_SetPosition(World *world, EntityID entityID, float x, float y);

void WorldEntity_SetVelocity(World *world, EntityID entityID, float dx,
                             float dy);

void WorldEntity_SetText(World *world, EntityID entityID, Text text);

void WorldEntity_SetPuck(World *world, EntityID entityID, Puck puck);

// void InputPull(Input *input, Camera2D camera);
void InputPull(Input *input, Camera2D camera, int cellSize);

void InputApply(Input input);

// void GameUpdate(World *world, Input *input, int cellSize, float dt);

void GameRun(GameConfig *config);

EntityID CreatePuck(World *world, Puck puck, int xCellIdx, int yCellIdx,
                    int cellSize, int cellSize_half);
void EntityVelocitySet(World *world, EntityID entityID, float dx, float dy);
void EntityVelocityApply(World *world, EntityID entityID, float dt);
void GameUpdate(World *world, Input *input, int cellSize, int cellSize_half,
                float dt);

Puck CheckForWin(World *world, int puckGridIndex, Puck puckValue);
#endif
/* vim:set ts=3 sw=2 sts=2 et: */
