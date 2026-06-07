#include "game.h"
#include "raylib.h"
#include "render.h"
#include <math.h>
#include <stdbool.h>

void EntityVelocitySet(World *world, EntityID entityID, float dx, float dy) {
  world->dx[entityID] = dx;
  world->dy[entityID] = dy;
}

void EntityVelocityApply(World *world, EntityID entityID, float dt) {
  world->x[entityID] += (world->dx[entityID] * dt);
  world->y[entityID] += (world->dy[entityID] * dt);
}

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

void GameUpdate(World *world, Input *input, int cellSize, int cellSize_half,
                float dt) {
  if (input->keyPressed_F) {
    ToggleFullscreen();
  }

  int x = input->mouseWorldPositionQuantized.x;
  int y = input->mouseWorldPositionQuantized.y;

  if (input->mouseLeftPressed && !world->puckFalling) {
    if (world->currentPuckTeam == PUCK_NIL) {
      world->currentPuckTeam = PUCK_BLUE;
    } else {
      world->currentPuckTeam =
          world->currentPuckTeam == PUCK_BLUE ? PUCK_RED : PUCK_BLUE;
    }

    if (x >= 0 && x < (GAME_COLUMN * cellSize) && y >= 0 &&
        y < (GAME_ROW * cellSize)) {
      int column = x / cellSize;
      if (world->columnStopPosition[column] < GAME_ROW) {
        int entityID = CreatePuck(world, world->currentPuckTeam, x, 0, cellSize,
                                  (cellSize / 2));
        EntityVelocitySet(world, entityID, 0, GRAVITY);

        world->puckColumnIndex[entityID] = column;
        world->puckFalling = true;
        world->currentPuckIndex = entityID;
      }
    }
  }

  if (world->puckFalling && world->currentPuckIndex != NIL) {
    EntityVelocityApply(world, world->currentPuckIndex, dt);

    int column = world->puckColumnIndex[world->currentPuckIndex];
    int stopPosition =
        ((GAME_ROW - world->columnStopPosition[column]) * cellSize) -
        cellSize_half;

    if (world->y[world->currentPuckIndex] >= stopPosition) {
      world->dy[world->currentPuckIndex] = 0;
      world->y[world->currentPuckIndex] = stopPosition;
      world->columnStopPosition[column]++;

      world->puckFalling = false;

      int gridIdx =
          ((stopPosition) / cellSize) * GAME_ROW + column;
      world->grid[gridIdx] = world->currentPuckTeam;
    }
  }
}

EntityID CreatePuck(World *world, Puck puck, int posX, int posY, int cellSize,
                    int cellSize_half) {
  EntityID entityID = WorldEntity_Create(world);

  WorldEntity_SetPosition(world, entityID, posX + cellSize_half,
                          posY + cellSize_half);

  WorldEntity_SetPuck(world, entityID, puck);

  Traits traits = TRAITS_PUCK | TRAITS_POSITIONABLE | TRAITS_PHYSICS;
  WorldEntity_TraitsAdd(world, entityID, traits);

  return entityID;
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

    GameUpdate(&world, &input, cellSize, cellSize_half, dt);
    RenderUpdate(&world, &input, camera, cellSize, lineWidth, puckRadius);
  }

  CloseWindow();
}

/* vim:set ts=3 sw=2 sts=2 et: */
