// assuming homebrew installation of raylib, compile with:
/* gcc main.c -o sand \
  -I/opt/homebrew/opt/raylib/include \
  -L/opt/homebrew/opt/raylib/lib \
  -lraylib \
  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
  */

/*
Controls:
Left click: place material
Right click: remove material
'1': sand
'2': water
'3': lava
'4': stone
'q': brush size 1x1
'w': brush 3x3
'e': brush 5x5
'c': clear all
'esc': close
*/


#include "raylib.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define GRID_WIDTH 100
#define GRID_HEIGHT 75
#define SCALE 4

#define EMPTY 0
#define SAND 1
#define WATER 2
#define STONE 3
#define LAVA 4

#define RATE 0.05f

int grid[GRID_HEIGHT][GRID_WIDTH] = {0};

Color sandColor  = (Color){255, 200, 150, 255};
Color waterColor = (Color){100, 100, 255, 255};
Color stoneColor = (Color){150, 150, 150, 255};
Color lavaColor = (Color){255, 80, 0, 255};

int currentMaterial = SAND;

int brushSize = 5;

int main(void) {
    InitWindow(GRID_WIDTH * SCALE, GRID_HEIGHT * SCALE, "Sand");
    SetTargetFPS(60);

    float spawnTimer = 0.0f;

    while (!WindowShouldClose()) {
        // User input: keyboard
        if (IsKeyPressed(KEY_ONE)) currentMaterial = SAND;
        if (IsKeyPressed(KEY_TWO)) currentMaterial = WATER;
        if (IsKeyPressed(KEY_THREE)) currentMaterial = LAVA;
        if (IsKeyPressed(KEY_FOUR)) currentMaterial = STONE;

        if (IsKeyPressed(KEY_Q)) brushSize = 1;
        if (IsKeyPressed(KEY_W)) brushSize = 3;
        if (IsKeyPressed(KEY_E)) brushSize = 5;

        if (IsKeyPressed(KEY_C)) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid[y][x] = EMPTY;
                }
            }
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            return 0;
        }

        // User input: left click
        spawnTimer += GetFrameTime();
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && spawnTimer >= RATE) {
            Vector2 mouse = GetMousePosition();
            int x = (int)(mouse.x / SCALE);
            int y = (int)(mouse.y / SCALE);

            for (int dy = -brushSize/2; dy <= brushSize/2; dy++) {
                for (int dx = -brushSize/2; dx <= brushSize/2; dx++) {
                    int px = x + dx;
                    int py = y + dy;
                    if (px >= 0 && px < GRID_WIDTH && py >= 0 && py < GRID_HEIGHT) {
                        grid[py][px] = currentMaterial;
                    }
                }
            }
            spawnTimer = 0.0f;
        }

        // Right click
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int x = (int)(mouse.x / SCALE);
            int y = (int)(mouse.y / SCALE);

            for (int dy = -brushSize/2; dy <= brushSize/2; dy++) {
                for (int dx = -brushSize/2; dx <= brushSize/2; dx++) {
                    int px = x + dx;
                    int py = y + dy;
                    if (px >= 0 && px < GRID_WIDTH && py >= 0 && py < GRID_HEIGHT) {
                        grid[py][px] = EMPTY;
                    }
                }
            }
        }

        // Physics
        for (int y = GRID_HEIGHT - 2; y >= 0; y--) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                // SAND
                if (grid[y][x] == SAND) {
                    if (grid[y+1][x] == EMPTY || grid[y+1][x] == WATER) {
                        grid[y][x] = grid[y+1][x]; // swap (sand pushes water down)
                        grid[y+1][x] = SAND;
                    } else if (x > 0 && (grid[y+1][x-1] == EMPTY || grid[y+1][x-1] == WATER)) {
                        grid[y][x] = grid[y+1][x-1];
                        grid[y+1][x-1] = SAND;
                    } else if (x < GRID_WIDTH-1 && (grid[y+1][x+1] == EMPTY || grid[y+1][x+1] == WATER)) {
                        grid[y][x] = grid[y+1][x+1];
                        grid[y+1][x+1] = SAND;
                    }
                }
                // WATER
                else if (grid[y][x] == WATER) {
                    if (grid[y+1][x] == EMPTY) {
                        grid[y+1][x] = WATER;
                        grid[y][x] = EMPTY;
                    } else {
                        int dir = GetRandomValue(0, 1) * 2 - 1; // -1 or +1, pick randomly for more natural movement
                        if (x + dir >= 0 && x + dir < GRID_WIDTH && grid[y][x + dir] == EMPTY) {
                            grid[y][x + dir] = WATER;
                            grid[y][x] = EMPTY;
                        }
                    }
                    // Interaction with lava
                    if (grid[y+1][x] == LAVA) {
                        grid[y+1][x] = STONE;
                    }
                }
                // LAVA
                else if (grid[y][x] == LAVA) {
                    if (grid[y+1][x] == EMPTY) {
                        grid[y+1][x] = LAVA;
                        grid[y][x] = EMPTY;
                    } else {
                        int dir = GetRandomValue(0, 1) * 2 - 1;
                        if (x + dir >= 0 && x + dir < GRID_WIDTH && grid[y][x + dir] == EMPTY) {
                            grid[y][x + dir] = LAVA;
                            grid[y][x] = EMPTY;
                        }
                    }
                    // Interaction with water
                    if (grid[y+1][x] == WATER) {
                        grid[y+1][x] = STONE;
                    }
                    // Interaction with sand
                    if (y+1 < GRID_HEIGHT && grid[y+1][x] == SAND && GetRandomValue(0, 100) < 5) {
                        grid[y+1][x] = LAVA;
                    }
                    if (x > 0 && grid[y][x-1] == SAND && GetRandomValue(0, 100) < 5) {
                        grid[y][x-1] = LAVA;
                    }
                    if (x < GRID_WIDTH-1 && grid[y][x+1] == SAND && GetRandomValue(0, 100) < 5) {
                        grid[y][x+1] = LAVA;
                    }
                    // Slowly melt nearby stone into lava
                    if (y+1 < GRID_HEIGHT && grid[y+1][x] == STONE && GetRandomValue(0, 100) < 1) {
                        grid[y+1][x] = LAVA;
                    }
                    if (y > 0 && grid[y-1][x] == STONE && GetRandomValue(0, 100) < 1) {
                        grid[y-1][x] = LAVA;
                    }
                    if (x > 0 && grid[y][x-1] == STONE && GetRandomValue(0, 100) < 1) {
                        grid[y][x-1] = LAVA;
                    }
                    if (x < GRID_WIDTH-1 && grid[y][x+1] == STONE && GetRandomValue(0, 100) < 1) {
                        grid[y][x+1] = LAVA;
                    }
                }
            }
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                    Color c;
                    switch(grid[y][x]) {
                        case SAND:  c = sandColor;  break;
                        case WATER: c = waterColor; break;
                        case STONE: c = stoneColor; break;
                        case LAVA:  c = lavaColor;  break;
                        default:    continue;
                    }
                    DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, c);
                }
        }
        // Label what material is selected
        const char* label = currentMaterial == SAND  ? "SAND"  :
                            currentMaterial == WATER ? "WATER" :
                            currentMaterial == STONE ? "STONE" :
                            currentMaterial == LAVA ? "LAVA" : "UNKNOWN";
        DrawText(TextFormat("Material: %s", label), 10, 10, 10, RAYWHITE);
        DrawText(TextFormat("Brush: %dx%d", brushSize, brushSize), 10, 25, 10, RAYWHITE);


        EndDrawing();
    }
    CloseWindow();
    return 0;
}

