#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#define MAX_BUILDINGS   100


// App Screens
typedef enum UIScreen {
    MAIN_MENU,
    SETTINGS,
    PROCESSOR,
    VIEWER,
    ERROR_PAGE
} GameScreen;


int launchUI() {

    // Initialization ==========================
    GameScreen currentScreen = MAIN_MENU;
    const int screenWidth = 800;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "Bustelo");
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    int frameCounter = 0;

    // Main app loop ===========================
    while (!WindowShouldClose()) {

        // Update ==============================
        switch(currentScreen) {
            case MAIN_MENU:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    // TODO: Open File Dialog
                    // file = OpenFileDialog("Select a file", NULL);
                    // currentScreen = PROCESSOR;
                    currentScreen = VIEWER;
                }
                break;
            case PROCESSOR:
                frameCounter++;
                // TODO: Actualy process the file
                if (frameCounter > 120) {
                    currentScreen = VIEWER;
                }
                break;
            case VIEWER:
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f/camera.zoom);
                    camera.target = Vector2Add(camera.target, delta);
                }

                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (wheel != 0)
                {
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
            case ERROR_PAGE:
                break;
            default:
                break;
        }

        // Draw Current Frame ==================
        BeginDrawing();
        switch(currentScreen) {
            case MAIN_MENU:
                ClearBackground(RAYWHITE);
                DrawText("Welcome to Bustelo - Drag and Drop a file to start", 20, 20, 20, BLACK);
                DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
                break;
            case PROCESSOR:
                ClearBackground(RAYWHITE);
                DrawText("Brewing", 20, 20, 20, BLACK);
                break;
            case VIEWER:
                ClearBackground(BLACK);
                BeginMode2D(camera);
                    // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
                    // TODO: Fill the grid with the data from the result of the Molido process
                    rlPushMatrix();
                        rlTranslatef(0, 25*50, 0);
                        rlRotatef(90, 1, 0, 0);
                        DrawGrid(100, 50);
                    rlPopMatrix();

                    // Vizual Reference
                    Rectangle rect = { 0, 0, 100, 100 };
                    DrawRectangleRec(rect, SKYBLUE);
                    
                EndMode2D();
                DrawText("Mouse right button drag to move, mouse wheel to zoom", 10, 10, 20, WHITE);
                break;
            case ERROR_PAGE:
                ClearBackground(RAYWHITE);
                DrawText("ERROR", 640, 10, 20, RED);
                DrawText("An error occured while processing the file", 20, 20, 20, BLACK);
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

    CloseWindow();

    // End ======================================
    return 0;
}

