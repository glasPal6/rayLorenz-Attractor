#include <stdlib.h>

#include "raylib.h"
#include "rlgl.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float GetRandomFloat(float from, float to) {
    return from + (to - from) * (float)GetRandomValue(0, INT32_MAX) / INT32_MAX;
}

void init_particles(uint32_t numParticles, Vector3* positions,
                    Vector3* velocities, Color* colors) {
    for (uint32_t i = 0; i < numParticles; i++) {
        positions[i] = (Vector3){
            GetRandomFloat(-50, 50),
            GetRandomFloat(-50, 50),
            GetRandomFloat(-50, 50),
        };
        velocities[i] = (Vector3){
            0,
            0,
            0,
        };
        colors[i] = (Color){
            (unsigned char)GetRandomValue(64, 255),
            (unsigned char)GetRandomValue(64, 255),
            (unsigned char)GetRandomValue(64, 255),
            255,
        };
    }
}

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
    uint32_t numParticles = 1024 * 1;
    // uint32_t numParticles = 512;
    Vector3* positions = malloc(sizeof(Vector3) * numParticles);
    Vector3* velocities = malloc(sizeof(Vector3) * numParticles);
    Color* colors = malloc(sizeof(Color) * numParticles);

    init_particles(numParticles, positions, velocities, colors);

    // Camera setup
    Camera camera = {
        {2, 2, 2},  // position
        {0, 0, 1},  // target (Lorenz tends to live in +Z)
        {0, 1, 0},
        45.0f,  // fovy
        CAMERA_PERSPECTIVE,
    };

    float time = 0;
    float timeScale = 0.2f;
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;
    float particleScale = 0.5;
    float positionScale = 0.05f;
    float instances_x1000 = 100.0;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        uint32_t numInstances =
            (uint32_t)(instances_x1000 / 1000 * numParticles);
        // UpdateCamera(&camera, CAMERA_FREE);
        UpdateCamera(&camera, CAMERA_ORBITAL);

        {
            // Compute Pass
            if (time == 0) {
                init_particles(numParticles, positions, velocities, colors);
            }

            for (uint32_t i = 0; i < numParticles; i++) {
                velocities[i].x = sigma * (positions[i].y - positions[i].x);
                velocities[i].y =
                    positions[i].x * (rho - positions[i].z) - positions[i].y;
                velocities[i].z =
                    positions[i].x * positions[i].y - beta * positions[i].z;

                positions[i].x += velocities[i].x * deltaTime * timeScale;
                positions[i].y += velocities[i].y * deltaTime * timeScale;
                positions[i].z += velocities[i].z * deltaTime * timeScale;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        {
            // Render Pass
            BeginMode3D(camera);
            for (uint32_t i = 0; i < numParticles; i++) {
                Vector3 p = positions[i];
                p.x *= positionScale;
                p.y *= positionScale;
                p.z *= positionScale;
                DrawSphere(p, 0.02f * particleScale, colors[i]);
            }
            EndMode3D();
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
