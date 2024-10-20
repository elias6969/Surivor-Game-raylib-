#ifndef GAMEHEADER_H
#define GAMEHEADER_H

#include <raylib.h>
#include <vector>

// Player structure
struct Player
{
    Vector2 PlayerPosition; // Player's position
    Vector2 playerSize;     // Size of the player
    float Speed;            // Speed of the player
    int widthofplayer;      // Width of the player
    int heightofplayer;     // Height of the player
    Texture2D texture;      // Texture of the player
};

// Enemies structure (if you want to store multiple enemies)
struct Enemies
{
    Vector2 Position; // Position of the enemy
    Color color;      // Color to represent the enemy
    const char *type; // Type of tree (e.g., "Oak", "Pine")
    Texture2D texture;
};

// Mineral structure
struct Mineral
{
    Vector2 position; // Position of the mineral
    Color color;      // Color to represent the type of mineral
    const char *type; // Type of mineral (e.g., "Gold", "Silver")
    Texture2D mineralTexture;
};

// Tree structure
struct Tree
{
    Vector2 position; // Position of the tree
    Color color;      // Color to represent the type of tree
    const char *type; // Type of tree (e.g., "Oak", "Pine")
    Texture2D treetexture;
};

struct TradeItem
{
    std::string name;
    int value; // Could represent how much the item is worth
};

// Villager structure
struct Villager
{
    Vector2 position; // Position of the villager
    Color color;      // Color to represent the villager
    const char *name; // Name of the villager
    bool canTrade;    // Indicates if the villager can trade
    Texture2D VillagerTexture;
    std::vector<TradeItem> tradeItems;
};

struct InventorySlot
{
    bool hasItem; // Does this slot have an item?
    const char *Itemtype;
    Rectangle rect; // Visual rectangle for the slot
    Color color;    // Color to represent item or empty
};

struct AIEntity
{
    Vector2 position;
    float speed;
    Vector2 direction;
    float directionChangeInterval;
    float timeSinceLastChange;
};

struct CollideExample
{
    Vector2 Position;
    int Width;
    int Height;
    Color color;
    const char *type;
};

typedef struct
{
    Rectangle rect;   // The rectangle representing the piece
    Vector2 velocity; // The velocity vector for the piece
    Color color;      // Color of the piece
    float lifetime;   // Time until the piece disappears
    const char *type; // Type of wall(if it can explode or not)
} ShatterPiece;
struct Rectangles
{
    float x;
    float y;
    float width;
    float height;
    const char *type;
};

struct villagerinventory
{
    Vector2 position; // Position of the mineral
    Color color;      // Color to represent the type of mineral
    const char *type; // Type of mineral (e.g., "Gold", "Silver")
    Texture2D texture;
};

extern AIEntity ai;

void UpdateAI(AIEntity &ai);
void DrawAI(const AIEntity &ai);

void init(std::vector<InventorySlot> &inventories, int slotsX, int slotsY, float slotSize, float slotPadding, int screenHeight);

void DrawInventory(const std::vector<InventorySlot> &inventories);

// Function declaration for PlayerCreation
void PlayerCreation(Player &player, std::vector<Enemies> &enemies, std::vector<Mineral> &minerals, std::vector<Tree> &trees, std::vector<Villager> &villagers, std::vector<InventorySlot> &inventory, bool &InteractedWithVillager);

void DrawEntities(const std::vector<Mineral> &minerals,
                  const std::vector<Tree> &trees,
                  const std::vector<Enemies> &enemies,
                  const std::vector<Villager> &villagers);

extern Texture2D Storemineraltexture;

void storeTexture(const char *filepath);

bool isMouseOverRectangle(Rectangle rect);

void WeaponSystem(Player &player, const std::vector<CollideExample> &Wall, bool &collided, bool &explod);

void villagerMineralsystem(std::vector<Mineral> minerals);

void DrawAI(const AIEntity &ai);

void Delete(const std::vector<Texture2D> &mineraltexture);

// Shatter animation part
void InitShatterPieces(Rectangle rect);
void UpdateShatterPieces(float deltaTime);
void DrawShatterPieces();

#endif