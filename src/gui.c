#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "logger.h"
#include <stdio.h>
#include <stdint.h>
#include "molido.h"

// App Screens
typedef enum UIScreen {
    MAIN_MENU,
    SETTINGS,
    PROCESSOR,
    VIEWER,
    ERROR_PAGE
} GameScreen;


// Files Handler ================================================================
#define MAX_FILEPATH_RECORDED   1
#define MAX_FILEPATH_SIZE       2048
typedef struct {
    int filepathCounter;
    char* filepaths[MAX_FILEPATH_RECORDED];
    MapType maps[MAX_FILEPATH_RECORDED];
    MapType originalMaps[MAX_FILEPATH_RECORDED];
} FilesHandler;

// Declaration of functions
void InitFileHandler(FilesHandler* fh);
void ResetFilesHandler(FilesHandler* fh);
void FreeFilesHandler(FilesHandler* fh);

// =============================================================================
#define TILE_SIZE 50


int launchUIEventLoop() {

    // Initialization ==========================================================
    GameScreen currentScreen = MAIN_MENU;
    const int screenWidth = 800;
    const int screenHeight = 650;
    SetTraceLogCallback(RaylibLogger);
    InitWindow(screenWidth, screenHeight, "Bustelo");
    SetTargetFPS(60);
    
    // Select file page ------------------------
    FilesHandler fh;
    InitFileHandler(&fh);

    // Viewer page ------------------------------
    int currentFile = 0;
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int frameCounter = 0;

    // Error page -------------------------------
    char* errorMessage = "";

    // Main app loop ============================
    while (!WindowShouldClose()) {

        // Update ===============================================================
        switch(currentScreen) {
            case MAIN_MENU:
                if (IsFileDropped()) {
                    FilePathList droppedFiles = LoadDroppedFiles();
                    for (int i = 0, offset = fh.filepathCounter; i < (int)droppedFiles.count; i++) {
                        if (fh.filepathCounter < (MAX_FILEPATH_RECORDED)) {
                            printf("[INFO]: Loading %s", fh.filepaths[offset + i]);
                            TextCopy(fh.filepaths[offset + i], droppedFiles.paths[i]);
                            fh.filepathCounter++;
                        } else {
                            currentScreen = ERROR_PAGE;
                            errorMessage = "Exceded the maximum number of file that can be processed";
                        }
                    }
                    UnloadDroppedFiles(droppedFiles);
                }

                if (IsKeyPressed(KEY_ENTER) && fh.filepathCounter > 0) {
                    currentScreen = PROCESSOR;
                }
                break;

            case PROCESSOR:
                frameCounter++;
                // WARN: Current implementation is blocking
                // Action take almost no delay for now
                for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
                    printf("[INFO] Processing: %s\n", fh.filepaths[i]);
                    fillMap(fh.filepaths[i], &(fh.maps[i]));
                    normaliseMap(&(fh.maps[currentFile]), 1);  // TODO: Introduce settings
                }
                currentScreen = VIEWER;

                break;

            case VIEWER:
                if (IsKeyPressed(KEY_Q)) {
                    ResetFilesHandler(&fh);
                    currentScreen = MAIN_MENU;
                    break;
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f/camera.zoom);
                    camera.target = Vector2Add(camera.target, delta);
                }
                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (wheel != 0) {
                    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                    camera.offset = GetMousePosition();
                    camera.target = mouseWorldPos;
                    const float zoomIncrement = 0.125f;
                    camera.zoom += (wheel*zoomIncrement);
                    if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
                }
                break;

            case ERROR_PAGE:
                // TODO: Reset all states send to initial Menu
                if (IsKeyPressed(KEY_ENTER)) {
                    ResetFilesHandler(&fh);
                    currentScreen = MAIN_MENU;
                }
                break;

            default:
                break;
        }

        // Draw Current Frame ==================================================
        BeginDrawing();
        switch(currentScreen) {
            case MAIN_MENU:
                if (fh.filepathCounter > 0) {
                    DrawText("PRESS ENTER TO PROCESS THE FILES", 120, 220, 20, DARKGREEN);
                }
                ClearBackground(RAYWHITE);
                if (fh.filepathCounter == 0) {
                    DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
                } else {
                    DrawText("Dropped files:", 100, 40, 20, DARKGRAY);
                    for (int i = 0; i < fh.filepathCounter; i++) {
                        if (i%2 == 0) DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                        else DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));
                        DrawText(fh.filepaths[i], 120, 100 + 40*i, 10, GRAY);
                    }
                    DrawText("Drop new files...", 100, 110 + 40*(fh.filepathCounter), 20, DARKGRAY);
                }
                break;

            case PROCESSOR:
                ClearBackground(RAYWHITE);
                DrawText("Brewing", 20, 20, 20, BLACK);
                break;

            case VIEWER:
                ClearBackground(LIGHTGRAY);
                BeginMode2D(camera);

                    MapType* currMap = &fh.maps[currentFile];             
                    for (int i = 0; i < MAP_SIZE; i++) {
                        for (int j = 0; j < MAP_SIZE; j++) {
                            uint32_t b = (*currMap)[i][j];
                            Color recColor;
                            recColor.r = b & 0xFF;
                            recColor.g = b & 0xFF;
                            recColor.b = b & 0xFF;
                            recColor.a = 255; 
                            DrawRectangle(i + TILE_SIZE, j + TILE_SIZE, TILE_SIZE, TILE_SIZE, recColor);
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

    // De-Initialization ========================================================
    FreeFilesHandler(&fh);
    
    // End ======================================================================
    CloseWindow();
    return 0;
}

// Implementation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void InitFileHandler(FilesHandler* fh) {
    fh->filepathCounter = 0;
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        // Allocate space for the required file paths
        fh->filepaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }
    for (int mapIdx = 0; mapIdx < MAX_FILEPATH_RECORDED; mapIdx++) {
        for (int i=0; i < MAP_SIZE; i++) {
            for (int j=0; j < MAP_SIZE; j++) {
                fh->maps[mapIdx][i][j] = 0;
                fh->originalMaps[mapIdx][i][j] = 0;
            }
        }
    }
}

void ResetFilesHandler(FilesHandler* fh) {
    fh->filepathCounter = 0;
    for (int mapIdx = 0; mapIdx < MAX_FILEPATH_RECORDED; mapIdx++) {
        for (int i=0; i < MAP_SIZE; i++) {
            for (int j=0; j < MAP_SIZE; j++) {
                fh->maps[mapIdx][i][j] = 0;
                fh->originalMaps[mapIdx][i][j] = 0;
            }
        }
    }
}

void FreeFilesHandler(FilesHandler* fh) {
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        RL_FREE(fh->filepaths[i]);
    }
}

