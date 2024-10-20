#ifndef MAPGENERATION
#define MAPGENERATION

#include <raylib.h>

// Define your Map structure
struct Map
{
    int row;               // Number of rows in the map
    int column;            // Number of columns in the map
    Vector2 Position;      // Position of the map (optional)
    Vector2 Tilesize;      // Size of each tile in the tileset
    Texture2D Tileset;     // The texture for the tileset
};

// Function declarations
void init(Map *mapinformation, const char *tilesetpath);
void Generate(Map *mapinformation, int tilespreadsheet[10][10], int tilesize, float scaleFactor, Texture2D tileset);  
void deleteTexture(Texture2D tileset);

#endif
