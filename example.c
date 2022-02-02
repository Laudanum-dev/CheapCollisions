// This basic example utilizes raylib (https://raylib.com) for drawing and etc.
// You get a window with a few solid rectangles (gray) and controlable player (red)
// Arrows to move the player
// 'W' to teleport player to cursor
// Player won't be moving if it's inside the rectangle

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "cheapCollisions.h"

int main(void)
{
    // Init window
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Collision check example");

    // Create obstacles - solid rigid rectangles
    Rectangle obstacles[] = {
        (Rectangle){ 100, 100, 100, 200 },
        (Rectangle){ 400, 50, 300, 100 },
        (Rectangle){ 500, 100, 100, 200 },
        (Rectangle){ 600, 300, 100, 100},
        (Rectangle){ 402, 20, 300, 100}
    };
    int obstaclesC = sizeof(obstacles) / sizeof(obstacles[0]);

    // Create Player - 40x40 rectangle
    Rectangle player = { 400, 250, 40, 40 };

    SetTargetFPS(60);

    // Create game loop
    while (!WindowShouldClose())    
    {
        // Get amount of seconds before the previous frame to make 
        // game not slowing down on low frame rate
        float deltaTime = GetFrameTime();

        // Get the vector player is to be moved by if there is no obstacles on its way
        float hV = 0.0f;
        float vV = 0.0f;
        if (IsKeyDown(KEY_LEFT)) hV -= 200.0f * deltaTime;
        if (IsKeyDown(KEY_RIGHT)) hV += 200.0f * deltaTime;
        if (IsKeyDown(KEY_UP)) vV -= 200.0f * deltaTime;
        if (IsKeyDown(KEY_DOWN)) vV += 200.0f * deltaTime;

        // Teleport-to-mouse key
        if (IsKeyPressed(KEY_W)) { 
            player.x = GetMouseX();
            player.y = GetMouseY();
        }

        // Calculate how much of motion will we do depending on the obstacles on our way
        Vector2 mV = { 1.0f, 1.0f };
        Vector2F V;
        for (int i = 0; i < obstaclesC; i++) {
            // For each obstacle on screen see how much it allows us to go in each axis
            V = rectWRect(
                obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height,
                player.x, player.y, player.width, player.height, hV, vV
            );
            // In result we will be stopped by "closest" obstacle, so we need to get minimum of each
            mV.x = fminf(V.x, mV.x);
            mV.y = fminf(V.y, mV.y);
        }
        player.x += mV.x * hV;
        player.y += mV.y * vV;
        // The code below will remove the player ability to "hug" the walls
        // player.x += min(mV.x, mV.y) * hV
        // player.y += min(mV.x, mV.y) * vV

        BeginDrawing();
            ClearBackground(LIGHTGRAY);

            // Draw obstacles
            for (int i = 0; i < obstaclesC; i++) {
                DrawRectangleRec(obstacles[i], (Color){155, 155, 155, 155});
                DrawRectangleLines(obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height, DARKGRAY);
            } 
            // Draw Player
            DrawRectangleRec(player, RED);  

            // Draw debug information in the corner
            char dbgStr[32];
            sprintf(dbgStr, "x: %f\ny: %f", player.x, player.y);
            DrawText(dbgStr, 10, 10, 20, GRAY);
            DrawFPS(10, 70);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
