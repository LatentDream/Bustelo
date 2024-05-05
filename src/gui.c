#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "logger.h"
#include <stdio.h>
#include "molido.h"

#define MAX_FILEPATH_RECORDED   1
#define MAX_FILEPATH_SIZE       2048

// App Screens
typedef enum UIScreen {
    MAIN_MENU,
    SETTINGS,
    PROCESSOR,
    VIEWER,
    ERROR_PAGE
} GameScreen;


// TO TEST SOME STUFF ===========================================================
#define GRID_SIZE 100
#define TILE_SIZE 50
void DrawGridBackground() {
    const int tile_size = 50;
    for (int i = -GRID_SIZE; i < GRID_SIZE; i ++) {
        DrawLineEx((Vector2){GRID_SIZE * TILE_SIZE, TILE_SIZE*i}, (Vector2){-GRID_SIZE * TILE_SIZE, TILE_SIZE*i}, 1, GRAY);
        DrawLineEx((Vector2){TILE_SIZE*i, GRID_SIZE * TILE_SIZE}, (Vector2){TILE_SIZE*i, -GRID_SIZE * TILE_SIZE}, 1, GRAY);
    }
}
void draw_axis(){
    DrawLineEx((Vector2){GRID_SIZE * TILE_SIZE, 0}, (Vector2){-GRID_SIZE * TILE_SIZE, 0}, 2, BLACK);
    DrawLineEx((Vector2){0, GRID_SIZE * TILE_SIZE}, (Vector2){0, -GRID_SIZE * TILE_SIZE}, 2, BLACK);
    for (int i = -GRID_SIZE; i < GRID_SIZE; i ++) {
        DrawText(TextFormat("%d", i * TILE_SIZE), i * TILE_SIZE + 5, 5, 5, BLACK);
        DrawText(TextFormat("%d", i * TILE_SIZE),5,  i * TILE_SIZE + 5, 5, BLACK);
    }
}
// =============================================================================


int launchUIEventLoop() {

    // Initialization ==========================
    GameScreen currentScreen = MAIN_MENU;
    const int screenWidth = 800;
    const int screenHeight = 650;
    SetTraceLogCallback(RaylibLogger);
    InitWindow(screenWidth, screenHeight, "Bustelo");
    SetTargetFPS(60);
    
    // Select file page ------------------------
    int filePathCounter = 0;
    char* filePaths[MAX_FILEPATH_RECORDED] = { 0 };
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        // Allocate space for the required file paths
        filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }
    MapType maps[MAX_FILEPATH_RECORDED] = {0};

    // Viewer page ------------------------------
    int currentFile = 0;
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int frameCounter = 0;

    // Error page -------------------------------
    char* errorMessage = "";

    // Main app loop ============================
    while (!WindowShouldClose()) {

        // Update ===============================
        switch(currentScreen) {
            case MAIN_MENU:
                if (IsFileDropped()) {
                    FilePathList droppedFiles = LoadDroppedFiles();
                    for (int i = 0, offset = filePathCounter; i < (int)droppedFiles.count; i++) {
                        if (filePathCounter < (MAX_FILEPATH_RECORDED)) {
                            printf("[INFO]: Loading %s", filePaths[offset + i]);
                            TextCopy(filePaths[offset + i], droppedFiles.paths[i]);
                            filePathCounter++;
                        } else {
                            currentScreen = ERROR_PAGE;
                            errorMessage = "Exceded the maximum number of file that can be processed";
                        }
                    }
                    UnloadDroppedFiles(droppedFiles);
                }

                if (IsKeyPressed(KEY_ENTER) && filePathCounter > 0) {
                    currentScreen = PROCESSOR;
                }
                break;

            case PROCESSOR:
                frameCounter++;
                // TODO: Run in a different thread - Current implementation is blocking
                // Action take almost no delay for now
                for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
                    printf("[INFO] Processing: %s\n", filePaths[i]);
                    fillMap(filePaths[i], &(maps[i]));
                    normaliseMap(&(maps[currentFile]), 0);  // TODO: Introduce settings
                }
                currentScreen = VIEWER;

                break;

            case VIEWER:
                // TODO: Listen for Esp and if it happen -> Reset state + Go to menu
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f/camera.zoom);
                    camera.target = Vector2Add(camera.target, delta);
                }
                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (wheel != 0) {
                    // Get the world point that is under the mouse
                    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                    // Set the offset to where the mouse is
                    camera.offset = GetMousePosition();
                    // Set the target to match, so that the camera maps the world space point 
                    // under the cursor to the screen space point under the cursor at any zoom
                    camera.target = mouseWorldPos;
                    // Zoom increment
                    const float zoomIncrement = 0.125f;
                    camera.zoom += (wheel*zoomIncrement);
                    if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
                }
                break;

            case ERROR_PAGE:
                // TODO: Reset all states send to initial Menu
                break;

            default:
                break;
        }

        // Draw Current Frame ==================
        BeginDrawing();
        switch(currentScreen) {
            case MAIN_MENU:
                if (filePathCounter > 0) {
                    DrawText("PRESS ENTER TO PROCESS THE FILES", 120, 220, 20, DARKGREEN);
                }
                ClearBackground(RAYWHITE);
                if (filePathCounter == 0) {
                    DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
                } else {
                    DrawText("Dropped files:", 100, 40, 20, DARKGRAY);
                    for (int i = 0; i < filePathCounter; i++) {
                        if (i%2 == 0) DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                        else DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));
                        DrawText(filePaths[i], 120, 100 + 40*i, 10, GRAY);
                    }
                    DrawText("Drop new files...", 100, 110 + 40*filePathCounter, 20, DARKGRAY);
                }
                break;

            case PROCESSOR:
                ClearBackground(RAYWHITE);
                DrawText("Brewing", 20, 20, 20, BLACK);
                break;

            case VIEWER:
                ClearBackground(BLACK);
                BeginMode2D(camera);

                    MapType* currMap = &maps[currentFile];             
                    for (int i = 0; i < MAP_SIZE; i++) {
                        for (int j = 0; j < MAP_SIZE; j++) {
                            Color c;
                            if ((*currMap)[i][j] > 26) {
                                c = SKYBLUE;
                            } else {
                                c = RED;
                            }
                            DrawRectangle(i + TILE_SIZE, j + TILE_SIZE, TILE_SIZE, TILE_SIZE, c);

                        }
                    }
                    
                EndMode2D();
                DrawText("Mouse right button drag to move, mouse wheel to zoom", 10, 10, 20, WHITE);
                break;

            case ERROR_PAGE:
                ClearBackground(RAYWHITE);
                DrawText("ERROR", 700, 10, 20, RED);
                DrawText(errorMessage, screenWidth/2, screenHeight/2, 20, BLACK);
                DrawRectangle(0, 0, screenWidth, 5, RED);
                DrawRectangle(0, 5, 5, screenHeight - 10, RED);
                DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
                DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);
                break;

            default:
                ClearBackground(SKYBLUE);
                DrawText("UnImplemented, come back later aligator", 20, 20, 20, BLACK);
                break;
        }
        EndDrawing();
    }

    // De-Initialization ========================
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        RL_FREE(filePaths[i]);
    }

    CloseWindow();

    // End ======================================
    return 0;
}

