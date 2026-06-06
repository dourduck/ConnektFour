#include "game.h"
#include "raylib.h"
#include "render.h"
#include <math.h>
#include <stdbool.h>

// EntityID CreatePuck(World *world, Puck puck, int xCellIdx, int yCellIdx, int
// cellSize, int cellSize_half);

World World_Create() { return (World){.nextID = 1}; }

EntityID WorldEntity_Create(World *world) {
  if (world->nextID <= MAX_ENTITIES) {
    EntityID entityID = world->nextID++;
    world->active[entityID] = true;
    return entityID;
  } else {
    return NIL;
  }
}

void WorldEntity_TraitsAdd(World *world, EntityID entityID, TraitFlag flag) {
  world->traits[entityID] |= flag;
}

void WorldEntity_TraitsRemove(World *world, EntityID entityID, TraitFlag flag) {
  world->traits[entityID] &= ~(flag);
}

void WorldEntity_TraitsClear(World *world, EntityID entityID) {
  world->traits[entityID] = TRAITS_NIL;
}

bool WorldEntity_HasTrait(World *world, EntityID entityID, TraitFlag flag) {
  return (world->traits[entityID] & flag) == flag;
}

void WorldEntity_Remove(World *world, EntityID entityID) {
  world->active[entityID] = false;
  world->x[entityID] = NIL;
  world->y[entityID] = NIL;
  world->dx[entityID] = NIL;
  world->dy[entityID] = NIL;
  world->text[entityID] = NIL;
  world->puck[entityID] = PUCK_NIL;
  world->traits[entityID] = TRAITS_NIL;
}

void WorldEntity_SetPosition(World *world, EntityID entityID, float x,
                             float y) {
  world->x[entityID] = x;
  world->y[entityID] = y;
}

void WorldEntity_SetVelocity(World *world, EntityID entityID, float dx,
                             float dy) {
  world->dx[entityID] = dx;
  world->dy[entityID] = dy;
}

void WorldEntity_SetText(World *world, EntityID entityID, Text text) {
  world->text[entityID] = text;
}

void WorldEntity_SetPuck(World *world, EntityID entityID, Puck puck) {
  world->puck[entityID] = puck;
}

void InputPull(Input *input, Camera2D camera, int cellSize) {
  Vector2 mouseWorldPosition = GetScreenToWorld2D(GetMousePosition(), camera);
  input->mouseWorldPosition.x = mouseWorldPosition.x;
  input->mouseWorldPosition.y = mouseWorldPosition.y;
  input->keyPressed_F = IsKeyPressed(KEY_F);
  input->mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
  input->keyPressed_GRAVE = IsKeyPressed(KEY_GRAVE);

  int x = (int)(floor(mouseWorldPosition.x / cellSize)) * cellSize;
  int y = (int)(floor(mouseWorldPosition.y / cellSize)) * cellSize;
  input->mouseWorldPositionQuantized.x = x;
  input->mouseWorldPositionQuantized.y = y;
}

// void InputApply(Input input) { }

void GameUpdate(World *world, Input *input, int cellSize, float dt) {
  if (input->keyPressed_F) {
    ToggleFullscreen();
  }

  int x = input->mouseWorldPositionQuantized.x;
  int y = input->mouseWorldPositionQuantized.y;

  if (x >= 0 && x < (GAME_COLUMN * cellSize) && y >= 0 &&
      y < (GAME_ROW * cellSize)) {
  }

  if (input->mouseLeftPressed) {
    if (x >= 0 && x < (GAME_COLUMN * cellSize) && y >= 0 &&
        y < (GAME_ROW * cellSize)) {
      CreatePuck(world, PUCK_RED, x, y, cellSize, (cellSize / 2));
    }
  }
}

EntityID CreatePuck(World *world, Puck puck, int posX, int posY, int cellSize,
                    int cellSize_half) {
  EntityID entityID = WorldEntity_Create(world);

  WorldEntity_SetPosition(world, entityID, posX + cellSize_half,
                          posY + cellSize_half);

  WorldEntity_SetPuck(world, entityID, puck);

  Traits traits = TRAITS_PUCK | TRAITS_POSITIONABLE;
  WorldEntity_TraitsAdd(world, entityID, traits);
}

void GameRun(GameConfig *config) {
  World world = World_Create();

  SetConfigFlags(FLAG_FULLSCREEN_MODE);

  int screenWidth = config->screenWidth;
  int screenHeight = config->screenHeight;
  const char *title = config->gameTitle;
  int cellSize = config->cellSize;

  InitWindow(screenWidth, screenHeight, title);
  SetExitKey(KEY_Q);

  int width = GetRenderWidth();
  int height = GetRenderHeight();
  Vector2 offset = {(int)(width / 2) - (int)(cellSize * GAME_COLUMN / 2),
                    (int)(height / 2) - (int)(cellSize * GAME_ROW / 2)};

  Camera2D camera = {.offset = offset, .zoom = 1.0};
  Input input;
  SetTargetFPS(60);

  int stride = 7;
  int lineWidth = 4;
  int cellSize_half = cellSize / 2;
  int puckRadius = cellSize_half - (int)(lineWidth * 1.5);

  // Puck Creation
  // for (int i = 0; i < (stride * stride); i++) {
  //   EntityID entityID = WorldEntity_Create(&world);
  //
  //   int x = (i % stride);
  //   int y = (i / stride);
  //
  //   WorldEntity_SetPosition(&world, entityID, (x * cellSize) + cellSize_half,
  //                           (y * cellSize) + cellSize_half);
  //
  //   Puck puck = (x + y) % 2 == 0 ? PUCK_BLUE : PUCK_RED;
  //   WorldEntity_SetPuck(&world, entityID, puck);
  //
  //   Traits traits = TRAITS_PUCK | TRAITS_POSITIONABLE;
  //   WorldEntity_TraitsAdd(&world, entityID, traits);
  // }

  // Cell Creation
  for (int i = 0; i < (stride * stride); i++) {
    EntityID entityID = WorldEntity_Create(&world);

    int x = (i % stride);
    int y = (i / stride);

    WorldEntity_SetPosition(&world, entityID, (x * cellSize), (y * cellSize));

    Traits traits = TRAITS_CELL | TRAITS_POSITIONABLE;
    WorldEntity_TraitsAdd(&world, entityID, traits);
  }

  while (!WindowShouldClose()) {
    InputPull(&input, camera, cellSize);

    if (input.keyPressed_GRAVE == true) {
      world.console = !world.console;
    }

    float dt = GetFrameTime();

    GameUpdate(&world, &input, cellSize, dt);
    RenderUpdate(&world, &input, camera, cellSize, lineWidth, puckRadius);
  }

  CloseWindow();
}

/* vim:set ts=3 sw=2 sts=2 et: */
