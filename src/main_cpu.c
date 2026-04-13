#include "raylib.h"
#include "rlgl.h"

int main() {
    const int screenWidth = 900;
    const int screenHeight = 500;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "PatienceMax");
    int fbWidth = rlGetFramebufferWidth() * 2;
    int fbHeight = rlGetFramebufferHeight() * 2;
    rlViewport(0, 0, fbWidth, fbHeight);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Raylib + ANGLE + Metal", 190, 200, 20, LIGHTGRAY);
            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
