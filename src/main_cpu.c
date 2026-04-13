#include <stdlib.h>

#include "raylib.h"
#include "rlgl.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float GetRandomFloat(float from, float to) {
    return from + (to - from) * (float)GetRandomValue(0, INT32_MAX) / INT32_MAX;
}

void init_particles(uint32_t numParticles, Vector4* positions,
                    Vector4* velocities) {
    for (uint32_t i = 0; i < numParticles; i++) {
        positions[i] = (Vector4){
            GetRandomFloat(-0.5, 0.5),
            GetRandomFloat(-0.5, 0.5),
            GetRandomFloat(-0.5, 0.5),
            0,
        };
        velocities[i] = (Vector4){
            0,
            0,
            0,
            0,
        };
    }
}
void step_particles() {}

int main() {
    const int screenWidth = 900;
    const int screenHeight = 500;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "CPU Particles");

    int fbWidth = rlGetFramebufferWidth() * 2;
    int fbHeight = rlGetFramebufferHeight() * 2;
    rlViewport(0, 0, fbWidth, fbHeight);

    SetTargetFPS(60);

    // Particles setup
    uint32_t numParticles = 1024 * 10;
    Vector4* positions = malloc(sizeof(Vector4) * numParticles);
    Vector4* velocities = malloc(sizeof(Vector4) * numParticles);

    init_particles(numParticles, positions, velocities);

    // Camera setup
    Camera camera = {
        {2, 2, 2}, {0, 0, 0}, {0, 1, 0}, 35.0, CAMERA_PERSPECTIVE,
    };

    float time = 0;
    float timeScale = 0.2f;
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;
    float particleScale = 1.0;
    float instances_x1000 = 100.0;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        uint32_t numInstances =
            (uint32_t)(instances_x1000 / 1000 * numParticles);
        UpdateCamera(&camera, CAMERA_ORBITAL);

        {
            // Compute Pass
        }

        BeginDrawing();
        ClearBackground(BLACK);

        {
            // Render Pass
        }

        {
            // GUI Pass
            GuiSlider((Rectangle){550, 10, 200, 10}, "Particles x1000",
                      TextFormat("%.2f", instances_x1000), &instances_x1000, 0,
                      1000);
            GuiSlider((Rectangle){550, 25, 200, 10}, "Particle Scale",
                      TextFormat("%.2f", particleScale), &particleScale, 0, 5);
            GuiSlider((Rectangle){550, 40, 200, 10}, "Speed",
                      TextFormat("%.2f", timeScale), &timeScale, 0, 1.0);
            GuiSlider((Rectangle){650, 70, 100, 10}, "Sigma",
                      TextFormat("%2.1f", sigma), &sigma, 0, 20);
            GuiSlider((Rectangle){650, 85, 100, 10}, "Rho",
                      TextFormat("%2.1f", rho), &rho, 0, 30);
            GuiSlider((Rectangle){650, 100, 100, 10}, "Beta",
                      TextFormat("%2.1f", beta), &beta, 0, 10);

            time += deltaTime;
            if (GuiButton((Rectangle){350, 10, 100, 20}, "Restart (Space)") ||
                IsKeyPressed(KEY_SPACE)) {
                time = 0;
            }
            if (GuiButton((Rectangle){280, 10, 60, 20}, "Reset")) {
                time = 0;
                timeScale = 0.2f;
                sigma = 10;
                rho = 28;
                beta = 8.0 / 3.0;
                particleScale = 1.0;
                instances_x1000 = 100.0;
            }

            DrawFPS(10, 10);
            DrawText(TextFormat("N=%d", numInstances), 10, 30, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
