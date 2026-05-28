#include "raylib.h"
#include <cmath>
#include <iostream>

// Window size adjusted for full screen 3D immersion
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768; 
const int MAP_WIDTH = 16;
const int MAP_HEIGHT = 16;
const int TILE_SIZE = 64;

// Day 5 Minimap scale reduction configuration factor
const float MINIMAP_SCALE = 0.25f; 

const int WORLD_MAP[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Player {
    float x;
    float y;
    float angle;
    float speed;
    float rotSpeed;
};

int main()
{ 
    std::cout << "[DEBUG] Booting Production Build..." << std::endl;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RetroRay Engine");
    SetTargetFPS(60);
    
    Player player = {120.0f, 120.0f, 0.0f, 150.0f, 2.5f};
    
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        if(IsKeyDown(KEY_A)) player.angle -= player.rotSpeed * deltaTime;
        if(IsKeyDown(KEY_D)) player.angle += player.rotSpeed * deltaTime;

        if (player.angle < 0) player.angle += 2 * PI;
        if (player.angle > 2 * PI) player.angle -= 2 * PI;
        
        float moveX = cosf(player.angle) * player.speed * deltaTime;
        float moveY = sinf(player.angle) * player.speed * deltaTime;
        const float COLLISION_PADDING = 12.0f;
        
        // Axis-Separated Collision Checking
        if(IsKeyDown(KEY_W))
        {
            float targetX = player.x + moveX;
            float targetY = player.y + moveY;
            float checkX = targetX + (moveX > 0 ? COLLISION_PADDING : -COLLISION_PADDING);
            float checkY = targetY + (moveY > 0 ? COLLISION_PADDING : -COLLISION_PADDING);

            if (WORLD_MAP[(int)(player.y / TILE_SIZE)][(int)(checkX / TILE_SIZE)] == 0) player.x = targetX;
            if (WORLD_MAP[(int)(checkY / TILE_SIZE)][(int)(player.x / TILE_SIZE)] == 0) player.y = targetY;
        }
        if(IsKeyDown(KEY_S))
        {
            float targetX = player.x - moveX;
            float targetY = player.y - moveY;
            float checkX = targetX - (moveX > 0 ? COLLISION_PADDING : -COLLISION_PADDING);
            float checkY = targetY - (moveY > 0 ? COLLISION_PADDING : -COLLISION_PADDING);

            if (WORLD_MAP[(int)(player.y / TILE_SIZE)][(int)(checkX / TILE_SIZE)] == 0) player.x = targetX;
            if (WORLD_MAP[(int)(checkY / TILE_SIZE)][(int)(player.x / TILE_SIZE)] == 0) player.y = targetY;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Render Full-Screen Primitives Backdrop (Ceiling and Floor splits)
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, Color{25, 25, 35, 255});
        DrawRectangle(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, Color{45, 45, 45, 255});
        
        const float FOV = 60.0f * (PI / 180.0f);
        const int NUM_RAYS = SCREEN_WIDTH; // Cast rays matching every single horizontal pixel column across the window!
        float startAngle = player.angle - FOV / 2.0f;
        
        for(int i = 0; i < NUM_RAYS; i++)
        {
            float rayAngle = startAngle + (i * FOV / NUM_RAYS);
            float distance = 0.0f;
            float stepSize = 1.0f;
            float rayX = player.x;
            float rayY = player.y;
            float cosRay = cosf(rayAngle);
            float sinRay = sinf(rayAngle);
            
            while (distance < 1000.0f)
            {
                rayX += cosRay * stepSize;
                rayY += sinRay * stepSize;
                distance += stepSize;

                int mapCheckX = (int)(rayX / TILE_SIZE);
                int mapCheckY = (int)(rayY / TILE_SIZE);

                if (mapCheckX >= 0 && mapCheckX < MAP_WIDTH && mapCheckY >= 0 && mapCheckY < MAP_HEIGHT) {
                    if (WORLD_MAP[mapCheckY][mapCheckX] == 1) break; 
                } else break;
            }
            
            float correctedDistance = distance * cosf(rayAngle - player.angle);
            if(correctedDistance < 1.0f) correctedDistance = 1.0f;
            
            // Scaled wall height projection factor to match new screen dimension properties
            float wallSliceHeight = (TILE_SIZE * 650.0f) / correctedDistance;
            if(wallSliceHeight > SCREEN_HEIGHT) wallSliceHeight = SCREEN_HEIGHT;
            
            float drawStart = (SCREEN_HEIGHT / 2.0f) - (wallSliceHeight / 2.0f);
            
            // --- DAY 5 DYNAMIC LIGHT SHADING CALCULATION ---
            float maxRenderDistance = 600.0f;
            float intensity = 1.0f - (correctedDistance / maxRenderDistance);
            if (intensity < 0.0f) intensity = 0.0f;
            if (intensity > 1.0f) intensity = 1.0f;

            // Generate a classic deeply-shaded blue palette mapping drop-off values
            Color wallColor = {
                (unsigned char)(25 * intensity),
                (unsigned char)(110 * intensity),
                (unsigned char)(200 * intensity),
                255
            };
            
            // Draw full screen width layout slices natively from left to right
            DrawLine(i, (int)drawStart, i, (int)(drawStart + wallSliceHeight), wallColor);
        }
        
        // --- DAY 5 MODULAR FLOATING MINIMAP HUD OVERLAY ---
        int offsetHUDX = 20;
        int offsetHUDY = 20;

        for(int y = 0; y < MAP_HEIGHT; y++)
        {
            for(int x = 0; x < MAP_WIDTH; x++)
            {
                Color tileColor = WORLD_MAP[y][x] == 1 ? Color{80, 80, 80, 200} : Color{0, 0, 0, 150};
                DrawRectangle(
                    offsetHUDX + (int)(x * TILE_SIZE * MINIMAP_SCALE),
                    offsetHUDY + (int)(y * TILE_SIZE * MINIMAP_SCALE),
                    (int)(TILE_SIZE * MINIMAP_SCALE) - 1,
                    (int)(TILE_SIZE * MINIMAP_SCALE) - 1,
                    tileColor
                );
            }
        }
        
        // Draw Scaled Player Token inside HUD minimap
        int miniPlayerX = offsetHUDX + (int)(player.x * MINIMAP_SCALE);
        int miniPlayerY = offsetHUDY + (int)(player.y * MINIMAP_SCALE);
        DrawCircle(miniPlayerX, miniPlayerY, 4, RED);
        DrawLine(miniPlayerX, miniPlayerY, 
                 miniPlayerX + (int)(cosf(player.angle) * 12), 
                 miniPlayerY + (int)(sinf(player.angle) * 12), RED);

        // Performance Text UI Placement
        DrawFPS(SCREEN_WIDTH - 100, 20);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}