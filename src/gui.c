/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* UI for Dynamic Binary Visualization 
* - 2D View
* - 3D View (Work in Progress)
*
* FYI: This is unfinished Software, it's an excuse to try Raylib :)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "molido.h"

// App Screens
typedef enum UIScreen {
    MAIN_MENU,
    SETTINGS,
    PROCESSOR,
    VIEWER_2D,
    VIEWER_3D,
    ERROR_PAGE
} AppView;


// Files Handler ================================================================
#define MAX_FILEPATH_RECORDED   1
#define MAX_FILEPATH_SIZE       2048
typedef struct {
    int currentFile;
    int filepathCounter;
    char* filepaths[MAX_FILEPATH_RECORDED];
    MapType maps[MAX_FILEPATH_RECORDED];               // With transformation
    MapType originalMaps[MAX_FILEPATH_RECORDED];       // Heavy to computer
    Map3DType *maps3D[MAX_FILEPATH_RECORDED];          // With transformation
    Map3DType *originalMaps3D[MAX_FILEPATH_RECORDED];  // Heavy to computer
} FilesHandler;

// Declaration of functions
void InitFileHandler(FilesHandler* fh);
void ResetFilesHandler(FilesHandler* fh);
void FreeFilesHandler(FilesHandler* fh);
void ProcessFilesHandler(FilesHandler* fh);
void MapValueToHeatmapColor(Color* color, int value);
// =============================================================================


#define TILE_SIZE           50   // 2D Viewer
#define RENDER_THRESHOLD    10   // 2D Viewer Between 0 & 255


int launchUIEventLoop() {

    // Initialization ==========================================================
    AppView currentView = MAIN_MENU;
    const int screenWidth = 800;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "Bustelo");
    SetTargetFPS(60);

    FilesHandler fh;
    InitFileHandler(&fh);

    // VIEWE 2D
    Camera2D camera = { 0 };
    camera.zoom = 1.75f;

    // VIEWE 3D
    Camera camera3D = { 0 };
    camera3D.position = (Vector3){ 16.0f, 16.0f, 16.0f }; // Position
    camera3D.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Looking at point
    camera3D.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Up vector (rotation towards target)
    camera3D.fovy = 45.0f;                                // Field-of-view Y
    camera3D.projection = CAMERA_PERSPECTIVE;             // Projection type

    // Error page --------------------------------------------------------------
    char* errorMessage = "";

    // Main app loop ============================
    while (!WindowShouldClose()) {

        // Update ==============================================================
        switch(currentView) {
            case MAIN_MENU:
                if (IsFileDropped()) {
                    FilePathList droppedFiles = LoadDroppedFiles();
                    for (int i = 0, offset = fh.filepathCounter; i < (int)droppedFiles.count; i++) {
                        if (fh.filepathCounter < (MAX_FILEPATH_RECORDED)) {
                            printf("[INFO]: Loading %s", fh.filepaths[offset + i]);
                            TextCopy(fh.filepaths[offset + i], droppedFiles.paths[i]);
                            fh.filepathCounter++;
                        } else {
                            currentView = ERROR_PAGE;
                            errorMessage = "Exceded the maximum number of file that can be processed";
                        }
                    }
                    UnloadDroppedFiles(droppedFiles);
                }

                if (IsKeyPressed(KEY_ENTER) && fh.filepathCounter > 0) {
                    currentView = PROCESSOR;
                }
                break;

            case PROCESSOR:
                // WARN: Current implementation is blocking
                ProcessFilesHandler(&fh);
                currentView = VIEWER_2D;
                break;

            case VIEWER_2D:
                if (IsKeyPressed(KEY_Q)) {
                    ResetFilesHandler(&fh);
                    currentView = MAIN_MENU;
                    break;
                }
                if (IsKeyPressed(KEY_C)) {
                    currentView = VIEWER_3D;
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

            case VIEWER_3D:
                if (IsKeyPressed(KEY_Q)) {
                    ResetFilesHandler(&fh);
                    currentView = MAIN_MENU;
                    break;
                }
                if (IsKeyPressed(KEY_Z)) {
                    currentView = VIEWER_2D;
                    break;
                }
                UpdateCamera(&camera3D, CAMERA_ORBITAL);
                break;

            case ERROR_PAGE:
                // TODO: Reset all states send to initial Menu
                if (IsKeyPressed(KEY_ENTER)) {
                    ResetFilesHandler(&fh);
                    currentView = MAIN_MENU;
                }
                break;

            default:
                break;
        }

        // Draw Current Frame ==================================================
        BeginDrawing();
        switch(currentView) {
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
                DrawText("Processing the files...", 20, 20, 20, BLACK);
                break;

            case VIEWER_2D:
                ClearBackground(LIGHTGRAY);
                BeginMode2D(camera);

                    MapType* currMap = &fh.maps[fh.currentFile];
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
                DrawText("Mouse right button drag to move, mouse wheel to zoom", 10, 10, 20, BLACK);
                DrawText("C to swtich 3D Mode", 10, 30, 20, BLACK);
                DrawText("Q to go to Main Menu", 10, 50, 20, BLACK);
                break;

            case VIEWER_3D:
                ClearBackground(BLACK);
                BeginMode3D(camera3D);

                    // Point Cloud - will kill all performance 255**3 points
                    // TODO NEXT: Figure out the point cloud 
                    Map3DType* currMap3D = fh.maps3D[fh.currentFile];
                    for (int i = 0; i < MAP_SIZE; i++) {
                        for (int j = 0; j < MAP_SIZE; j++) {
                            for (int k = 0; k < MAP_SIZE; k++) { 
                                uint32_t b = (*currMap3D)[i][j][k];
                                if (b > RENDER_THRESHOLD) {
                                    Vector3 point;
                                    // Between 0 and MAP_SIZE
                                    point.x = (float)(i - MAP_SIZE/2)/20;
                                    point.y = (float)(j - MAP_SIZE/2)/20;
                                    point.z = (float)(k - MAP_SIZE/2)/20;

                                    Color color;
                                    MapValueToHeatmapColor(&color, b);
                                    DrawSphere(point, 0.05f, color);
                                }

                            }
                        }
                    }

                EndMode3D();
                DrawText("3D View", 10, 10, 20, WHITE);
                DrawText("Z to switch to 2D Mode", 10, 30, 20, WHITE);
                DrawText("Q to go to Main Menu", 10, 50, 20, WHITE);
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
                DrawText("UnImplemented, come back later!", 20, 20, 20, BLACK);
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
    fh->currentFile = 0;
    fh->filepathCounter = 0;
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        // Allocate space for the required file paths
        fh->filepaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        fh->maps3D[i] = malloc(MAP_SIZE * sizeof(int [MAP_SIZE][MAP_SIZE]));
        if (fh->maps3D[i] == NULL) {
            printf("[ERROR] Panik, can't allocate memory for maps3D\n");
        }
        fh->originalMaps3D[i] = malloc(MAP_SIZE * sizeof(int [MAP_SIZE][MAP_SIZE]));
        if (fh->originalMaps3D[i] == NULL) {
            printf("[ERROR] Panik, can't allocate memory for origialMaps3D\n");
        }
    }

    for (int mapIdx = 0; mapIdx < MAX_FILEPATH_RECORDED; mapIdx++) {
        Map3DType* originalMaps3D = fh->originalMaps3D[mapIdx];
        Map3DType* maps3D = fh->maps3D[mapIdx];
        for (int i=0; i < MAP_SIZE; i++) {
            for (int j=0; j < MAP_SIZE; j++) {
                fh->maps[mapIdx][i][j] = 0;
                fh->originalMaps[mapIdx][i][j] = 0;
                for (int k=0; k < MAP_SIZE; k++) {
                    (*maps3D)[i][j][k] = 0;
                    (*originalMaps3D)[i][j][k] = 0;
                }
            }
        }
    }
}

void ResetFilesHandler(FilesHandler* fh) {
    fh->currentFile = 0;
    fh->filepathCounter = 0;
    for (int mapIdx = 0; mapIdx < MAX_FILEPATH_RECORDED; mapIdx++) {
        Map3DType* originalMaps3D = fh->originalMaps3D[mapIdx];
        Map3DType* maps3D = fh->maps3D[mapIdx];
        for (int i=0; i < MAP_SIZE; i++) {
            for (int j=0; j < MAP_SIZE; j++) {
                fh->maps[mapIdx][i][j] = 0;
                fh->originalMaps[mapIdx][i][j] = 0;
                for (int k=0; k < MAP_SIZE; k++) {
                    (*maps3D)[i][j][k] = 0;
                    (*originalMaps3D)[i][j][k] = 0;
                }
            }
        }
    }
}

void FreeFilesHandler(FilesHandler* fh) {
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        RL_FREE(fh->filepaths[i]);
        free(fh->maps3D[i]);
        free(fh->originalMaps3D[i]);
    }
}

void ProcessFilesHandler(FilesHandler* fh) {
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        printf("[INFO] Processing: %s\n", fh->filepaths[i]);
        // 2D
        fillMap(fh->filepaths[i], &(fh->maps[i]));
        copyMap(&(fh->maps[i]), &(fh->originalMaps[i]));
        normaliseMap(&(fh->maps[fh->currentFile]), 1);
        // 3D
        fillMap3D(fh->filepaths[i], fh->maps3D[i]);
        copyMap3D(fh->maps3D[i], fh->originalMaps3D[i]);
        normaliseMap3D(fh->maps3D[fh->currentFile], 1);
    }
}

void MapValueToHeatmapColor(Color* color, int b) {
    if (b < 128) {
        color->r = 0;        // Low intensity, more towards blue
        color->g = 2 * b;
        color->b = 255 - 2 * b;
    } else {
        color->r = 2 * (b - 128);  // Higher intensity, more towards red
        color->g = 255 - 2 * (b - 128);
        color->b = 0;
    }
    color->a = 255; // Alpha value
}
