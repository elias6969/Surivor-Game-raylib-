#include <iostream>
#include <raylib.h>
#include "GameHeader.h"

#define MAX_PIECES 50

ShatterPiece pieces[MAX_PIECES];
int pieceCount = 0;

void InitShatterPieces(Rectangle rect)
{
    int rows = 5; // Number of rows you want
    int cols = 5; // Number of columns you want
    pieceCount = 0;

    float pieceWidth = rect.width / cols;   // Calculate width of each piece
    float pieceHeight = rect.height / rows; // Calculate height of each piece

    for (int i = 0; i < rows * cols; i++)
    {
        pieces[i].rect.width = pieceWidth;
        pieces[i].rect.height = pieceHeight;

        // Set position within the rectangle based on grid
        pieces[i].rect.x = rect.x + (i % cols) * pieceWidth;
        pieces[i].rect.y = rect.y + (i / cols) * pieceHeight;

        pieces[i].velocity.x = (GetRandomValue(-100, 1000) / 10.0f); // random X velocity
        pieces[i].velocity.y = (GetRandomValue(-100, 1000) / 10.0f); // random Y velocity

        // Set color and lifetime
        pieces[i].color = {255, 255, 255, 255}; // White color
        pieces[i].lifetime = 1.0f;              // Last for 1 second
        pieceCount++;
    }
}

void UpdateShatterPieces(float deltaTime)
{
    for (int i = 0; i < pieceCount; i++)
    {
        pieces[i].rect.x += pieces[i].velocity.x * deltaTime;
        pieces[i].rect.y += pieces[i].velocity.y * deltaTime;

        // Decrease lifetime
        pieces[i].lifetime -= deltaTime;
        if (pieces[i].lifetime < 0)
        {
            pieces[i].lifetime = 0; // Cap lifetime at 0
        }
    }
}

void DrawShatterPieces()
{
    for (int i = 0; i < pieceCount; i++)
    {
        // Set the alpha based on lifetime
        Color currentColor = {pieces[i].color.r, pieces[i].color.g, pieces[i].color.b, (unsigned char)(255 * (pieces[i].lifetime))};
        DrawRectangleRec(pieces[i].rect, currentColor);
    }
}
