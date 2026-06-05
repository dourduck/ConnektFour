#include "game.h"
#include "raylib.h"
#include <stdbool.h>

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

void InputPull(Input *input, Camera2D camera) {
  Vector2 mouseWorldPosition = GetScreenToWorld2D(GetMousePosition(), camera);
  input->mouseWorldPosition.x = mouseWorldPosition.x;
  input->mouseWorldPosition.y = mouseWorldPosition.y;
  input->keyPressed_F = IsKeyPressed(KEY_F);
}

void InputApply(Input input) {
  if (input.keyPressed_F) {
    ToggleFullscreen();
  }
}

void GameUpdate(World *world, float dt) {}

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

  for (int i = 0; i < (stride * stride); i++) {
    EntityID entityID = WorldEntity_Create(&world);

    int x = (i % stride);
    int y = (i / stride);

    WorldEntity_SetPosition(&world, entityID, (x * cellSize) + cellSize_half, (y * cellSize) + cellSize_half);

    Puck puck = (x + y) % 2 == 0 ? PUCK_BLUE : PUCK_RED;
    WorldEntity_SetPuck(&world, entityID, puck);

    Traits traits = TRAITS_PUCK | TRAITS_POSITIONABLE;
    WorldEntity_TraitsAdd(&world, entityID, traits);
  }


  for (int i = 0; i < (stride * stride); i++) {
    EntityID entityID = WorldEntity_Create(&world);

    int x = (i % stride);
    int y = (i / stride);

    WorldEntity_SetPosition(&world, entityID, (x * cellSize), (y * cellSize));

    Traits traits = TRAITS_CELL | TRAITS_POSITIONABLE;
    WorldEntity_TraitsAdd(&world, entityID, traits);
  }

  while (!WindowShouldClose()) {
    InputPull(&input, camera);
    InputApply(input);

    float dt = GetFrameTime();

    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode2D(camera);

    for (int i = 1; i <= world.nextID; i++) {
      TraitFlag flag = TRAITS_PUCK | TRAITS_POSITIONABLE;

      if (world.active[i] && WorldEntity_HasTrait(&world, i, flag)) {
        Puck puck = world.puck[i];

        float x = world.x[i];
        float y = world.y[i];

        if (puck == PUCK_BLUE) {
          DrawCircle(x, y, puckRadius, BLUE);
        } else if (puck == PUCK_RED) {
          DrawCircle(x, y, puckRadius, RED);
        } else {
          DrawCircle(x, y, puckRadius, MAGENTA);
        }
      }
    }

    for (int i = 1; i <= world.nextID; i++) {
      TraitFlag flag = TRAITS_CELL | TRAITS_POSITIONABLE;

      if (world.active[i] && WorldEntity_HasTrait(&world, i, flag)) {
        float x = world.x[i];
        float y = world.y[i];
        Rectangle r = {.x = x, .y = y, .width = cellSize, .height = cellSize};
        DrawRectangleLinesEx(r, lineWidth, DARKGRAY);
      }
    }

    EndMode2D();

    EndDrawing();
  }

  CloseWindow();
}

/* vim:set ts=3 sw=2 sts=2 et: */
