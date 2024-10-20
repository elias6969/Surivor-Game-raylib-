#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "MapGeneration.h"

Vector2 startposition;

void init(Map *mapinformation, const char *tilesetpath)
{
    // Load the tileset texture
    mapinformation->Tileset = LoadTexture(tilesetpath);

    // Define the size of each tile in the tileset (e.g., 32x32 pixels)
    mapinformation->Tilesize = {32.0f, 32.0f};

    // Define the starting position for drawing the grid
    startposition = {100.0f, 100.0f};
}

void Generate(Map *mapInformation, int tilespreadsheet[10][10], int tileSize, float scaleFactor, Texture2D tileset)
{
    int columnsInTileset = 30; // Number of columns in your 30x16 tileset
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            int tileIndex = tilespreadsheet[y][x];

            // Calculate the tile position in the tileset using the number of columns
            Rectangle tileRect = {
                (tileIndex % columnsInTileset) * tileSize, // X position in the tileset
                (tileIndex / columnsInTileset) * tileSize, // Y position in the tileset
                tileSize,                                  // Width of the tile
                tileSize                                   // Height of the tile
            };

            // Calculate scaled position on screen
            Vector2 tilePos = {
                (float)(x * tileSize * scaleFactor),
                (float)(y * tileSize * scaleFactor)};

            // Calculate the scaled tile size
            Vector2 tileSizeScaled = {tileSize * scaleFactor, tileSize * scaleFactor};

            // Draw the tile with scaling
            DrawTexturePro(tileset, tileRect,
                           (Rectangle){tilePos.x, tilePos.y, tileSizeScaled.x, tileSizeScaled.y},
                           (Vector2){0, 0}, 0.0f, WHITE);
        }
    }
}

void deleteTexture(Texture2D tileset)
{
    UnloadTexture(tileset); // Unload the texture when you're done with it
}
