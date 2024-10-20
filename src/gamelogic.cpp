#include <raylib.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>

#include "GameHeader.h"

std::vector<InventorySlot> inventories;
Texture2D goldTexture;
Texture2D SwordTexture;
Texture2D SilverTexture;
Texture2D UseitemSpreadSheet;

struct Tool {
    Rectangle rect;
};

Tool tools[6] = {
    { {0, 0, 32, 32} },   // Pickaxe
    { {32, 0, 32, 32} },  // Axe
    { {64, 0, 32, 32} },  // Sword
    { {96, 0, 32, 32} },  // Pot
    { {0, 32, 32, 32} },  // Hoe
    { {32, 32, 32, 32} }   // Harvest Tool
};

std::vector<villagerinventory> inventoryTrading = {
   {{0.0f, 0.0f}, GOLD, "gold", goldTexture},
   {{0.0f,0.0f}, LIGHTGRAY, "Sword", SwordTexture},
   {{0.0f,0.0f}, LIGHTGRAY, "Silver", SilverTexture},
   {{0.0f,0.0f}, BROWN, "Items", UseitemSpreadSheet}
};
bool pickedup = false;

bool isMouseOverRectangle(Rectangle rect)
{
    Vector2 mousePosition = GetMousePosition();
    return CheckCollisionPointRec(mousePosition, rect);
}
template<typename T>
void transferdata(const std::vector<T>& source, std::vector<T>& destination)
{
    destination.insert(destination.end(), source.begin(), source.end());
}

void init(std::vector<InventorySlot> &inventories, int slotsX, int slotsY, float slotSize, float slotPadding, int screenHeight)
{
    // Set up inventory grid at the bottom of the screen
    for (int x = 0; x < slotsX; x++)
    {
        InventorySlot slot;
        slot.hasItem = false; // Initially no item
        slot.rect = {
            (float)x * (slotSize + slotPadding) + 100, // X Position with padding
            (float)screenHeight - slotSize - 50,       // Y Position at bottom of the screen (50 is the padding from the bottom)
            (float)slotSize, (float)slotSize           // Size of each slot
        };
        slot.color = LIGHTGRAY;      // Default empty slot color
        inventories.push_back(slot); // Add slot to the vector
    }
    goldTexture = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Gold.png");
    SwordTexture = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Gold.png");
    SilverTexture = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Silver.png");
    UseitemSpreadSheet = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/UseItems.png");
    if(goldTexture.id == 0 or SwordTexture.id == 0 or UseitemSpreadSheet.id == 0 or SilverTexture.id == 0){
        printf("Failed To load in Texture!!");
    }else{
        printf("Succsefully loaded texture");
    }
}

void drawItem(const InventorySlot &slot)
{
    // Checks what type of object is picked up(This always updates So be carefull)
    if (slot.Itemtype == "Gold")
    {
        // gold.height = slot.rect.height - 2;
        // gold.width = slot.rect.width - 2;
        // DrawTexture(gold, slot.rect.x, slot.rect.y, WHITE);
    }
    else if (slot.Itemtype == "Silver")
    {
        DrawCircle(slot.rect.x + slot.rect.width / 2, slot.rect.y + slot.rect.height / 2, 10, LIGHTGRAY);
    }
    else if (slot.Itemtype == "Oak")
    {
    }
    else
    {
        std::cout << "couldnt find mineral type, fix the code dumbass " << std::endl;
    }
}

void DrawInventory(const std::vector<InventorySlot> &inventories)
{
    for (const auto &slot : inventories)
    {
        // Draw the inventory slot
        DrawRectangleRec(slot.rect, slot.color);

        // Checks what the name of the object picked up
        std::unordered_map<std::string, void (*)(const InventorySlot &)> itemAction = {
            {"Gold", drawItem},
            {"Silver", drawItem},
            {"Oak", drawItem}};

        if (slot.hasItem)
        {
            std::string itemTypestr = slot.Itemtype;

            if (itemAction.find(itemTypestr) != itemAction.end())
            {
                itemAction[itemTypestr](slot);
            }
            else
            {
                std::cout << "Error: unkown item!" << std::endl;
            }
        }
    }
}

// Helper function to calculate distance between two points
inline float CalculateDistance(const Vector2 &p1, const Vector2 &p2)
{
    return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}

// Templated function to handle interaction for all entities except Enemies
template <typename Entity>
void HandleInteraction(const Player &player, const Entity &entity, const Vector2 &interactionPoint, const char *action, float interactionRadius)
{
    float distance = CalculateDistance(entity.position, interactionPoint);
    if (distance <= interactionRadius && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        std::cout << action << " " << entity.type << std::endl;

        pickedup = true;
    }
}

// Overloaded function for Enemies since they use 'Position' instead of 'position'
void HandleInteraction(const Player &player, const Enemies &enemy, const Vector2 &interactionPoint, const char *action, float interactionRadius)
{
    float distance = CalculateDistance(enemy.Position, interactionPoint);
    if (distance <= interactionRadius && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        std::cout << action << " " << enemy.type << std::endl;
    }
}

// Templated function to draw any entity
template <typename Entity>
void DrawEntity(const Entity &entity)
{
    // Check if the entity has a texture and draw it
    if constexpr (std::is_same_v<Entity, Enemies>) // Check if Entity is of type Enemies
    {
        DrawTextureV(entity.texture, entity.Position, entity.color);
    }
    else
    {
        DrawCircleV(entity.position, 10.0f, entity.color); // For entities that have 'position'
    }

    DrawText(entity.type, entity.position.x - 15, entity.position.y - 15, 10, WHITE);
}

// Overloaded function for Enemies to use 'Position' field
void DrawEntity(const Enemies &enemy)
{
    DrawTextureV(enemy.texture, enemy.Position, enemy.color);
    DrawText(enemy.type, enemy.Position.x - 15, enemy.Position.y - 15, 10, WHITE);
    pickedup = true;
}

/*
 __      ___ _ _
 \ \    / (_) | |
  \ \  / / _| | | __ _  __ _  ___ _ __
   \ \/ / | | | |/ _` |/ _` |/ _ \ '__|
    \  /  | | | | (_| | (_| |  __/ |
     \/   |_|_|_|\__,_|\__, |\___|_|
                        __/ |
                       |___/
*/
// Special draw function for Villagers to use 'name' instead of 'type'
void DrawVillager(const Villager &villager)
{
    DrawCircleV(villager.position, 10.0f, villager.color);
    DrawText(villager.name, villager.position.x - 15, villager.position.y - 15, 10, WHITE);
}

void DrawTradeInterface(const Villager &villager, bool &villagerinteraction, int tradesize[4][3], int Tradtilesize)
{
    float scalefactor = 1.0f; // Scaling factor for size adjustments
    int rows = 3;
    int columns = 4;
    int offset = 25;

    // Drawing background for trade interface
    DrawRectangle(200, 200, 400, 300, Fade(GRAY, 0.8f)); // Semi-transparent background
    DrawText("Trade Interface", 280, 180, 24, WHITE); // Title for context

    // Draw the trade grid
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            // Calculate position for each box in the grid
            Vector2 tradepos = {
                220 + offset + (x * (Tradtilesize + 10) * scalefactor), // X position with some spacing
                230 + offset + (y * (Tradtilesize + 10) * scalefactor)  // Y position with some spacing
            };

            // Draw each grid box
            DrawRectangleV(tradepos, (Vector2){Tradtilesize * scalefactor, Tradtilesize * scalefactor}, Fade(DARKGRAY, 0.9f));

            // Calculate the index for the item
            int index = y * columns + x;

            if (index < inventoryTrading.size()) // Make sure we don't go out of bounds
            {
                // Draw the item type and value in the box
                DrawText(inventoryTrading[index].type, tradepos.x + 5, tradepos.y + 5, 20, WHITE); // Item type

                // Placeholder for value
                std::string valueText = "Value: " + std::to_string(100 * (index + 1)); // Sample value, replace with actual logic
                DrawText(valueText.c_str(), tradepos.x + 5, tradepos.y + 30, 18, YELLOW); // Item value in a different color
            }
        }
    }

    // Draw the villager's name
    DrawText(villager.name, 250, 220, 20, WHITE);

    // Draw the close button (X)
    Rectangle closeBtn = {550, 200, 40, 40};
    DrawRectangleRec(closeBtn, isMouseOverRectangle(closeBtn) ? MAROON : RED); // Button turns darker when hovered
    DrawText("X", 560, 210, 20, WHITE);

    if (isMouseOverRectangle(closeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        villagerinteraction = false;
    }
}

// Handle villager interaction with speech bubble and typing effect
void HandleVillagerInteraction(const Villager &villager, float interactionTime, bool &villagerinteraction)
{
    int grid[4][3];
    int tradesize = 70;
    std::vector<Mineral> minerals;
    DrawTradeInterface(villager, villagerinteraction, grid, tradesize);
}

/*
  _____  _                             __      _                     __                  _   _
 |  __ \| |                           / /     | |                   / _|                | | (_)
 | |__) | | __ _ _   _  ___ _ __     / /    __| |_ __ __ ___      _| |_ _   _ _ __   ___| |_ _  ___  _ __
 |  ___/| |/ _` | | | |/ _ \ '__|   / /    / _` | '__/ _` \ \ /\ / /  _| | | | '_ \ / __| __| |/ _ \| '_ \
 | |    | | (_| | |_| |  __/ |     / /    | (_| | | | (_| |\ V  V /| | | |_| | | | | (__| |_| | (_) | | | |
 |_|    |_|\__,_|\__, |\___|_|    /_/      \__,_|_|  \__,_| \_/\_/ |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|
                  __/ |
                 |___/
*/

// Function for player movement and interaction handling
void PlayerCreation(Player &player, std::vector<Enemies> &enemies, std::vector<Mineral> &minerals, std::vector<Tree> &trees, std::vector<Villager> &villagers, std::vector<InventorySlot> &inventory, bool &InteractedWithVillager)
{

    float deltaTime = GetFrameTime(); // Time step for consistent movement

    // Define interaction points around the player
    Vector2 UpPoint = {player.PlayerPosition.x, player.PlayerPosition.y - 50.0f};
    Vector2 DownPoint = {player.PlayerPosition.x, player.PlayerPosition.y + 50.0f};
    Vector2 RightPoint = {player.PlayerPosition.x + 50.0f, player.PlayerPosition.y};
    Vector2 LeftPoint = {player.PlayerPosition.x - 50.0f, player.PlayerPosition.y};

    // Handle player movement based on WASD keys
    if (IsKeyDown(KEY_W))
    {
        player.PlayerPosition.y -= player.Speed * deltaTime;
        DrawLineV(player.PlayerPosition, UpPoint, RED);
    }
    if (IsKeyDown(KEY_S))
    {
        player.PlayerPosition.y += player.Speed * deltaTime;
        DrawLineV(player.PlayerPosition, DownPoint, RED);
    }
    if (IsKeyDown(KEY_D))
    {
        player.PlayerPosition.x += player.Speed * deltaTime;
        DrawLineV(player.PlayerPosition, RightPoint, RED);
    }
    if (IsKeyDown(KEY_A))
    {
        player.PlayerPosition.x -= player.Speed * deltaTime;
        DrawLineV(player.PlayerPosition, LeftPoint, RED);
    }

    // Draw the player
    DrawCircleV(player.PlayerPosition, 20.0f, WHITE);

    // Handle interaction and drawing for enemies
    for (const Enemies &enemy : enemies)
    {
        HandleInteraction(player, enemy, RightPoint, "Attacking", 20.0f);
        DrawEntity(enemy);
    }

    // Handle interaction and drawing for minerals (with inventory system integration)
    for (int i = 0; i < minerals.size(); i++)
    {
        float distanceRight = CalculateDistance(minerals[i].position, RightPoint);
        float distanceLeft = CalculateDistance(minerals[i].position, LeftPoint);
        float distanceUp = CalculateDistance(minerals[i].position, UpPoint);
        float distanceDown = CalculateDistance(minerals[i].position, DownPoint);

        // If player clicks and is close enough to the mineral, pick it up
        if ((distanceRight <= 20.0f or distanceLeft <= 20.0f or distanceDown <= 20.0f or distanceUp <= 20.0f) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::cout << "Collecting " << minerals[i].type << std::endl;

            // Find an empty inventory slot
            bool slotFound = false;
            for (auto &slot : inventory)
            {
                if (!slot.hasItem)
                {
                    // Place the mineral in the empty slot
                    slot.hasItem = true;
                    slot.Itemtype = minerals[i].type;
                    slot.color = minerals[i].color; // Use mineral color to represent item
                    slotFound = true;
                    break; // Exit the loop once the item is placed
                }
            }

            // If inventory is full, we skip adding the mineral
            if (!slotFound)
            {
                std::cout << "Inventory is full!" << std::endl;
            }
            else
            {
                // Remove the mineral from the world after picking it up
                minerals.erase(minerals.begin() + i);
                i--; // Adjust the index after removing the element
            }
        }

        // Draw the remaining minerals
        if (i < minerals.size())
        {
            DrawEntity(minerals[i]);
        }
    }

    for (int i = 0; i < trees.size(); i++)
    {
        float distanceRight = CalculateDistance(trees[i].position, RightPoint);
        float distanceLeft = CalculateDistance(trees[i].position, LeftPoint);
        float distanceUp = CalculateDistance(trees[i].position, UpPoint);
        float distanceDown = CalculateDistance(trees[i].position, DownPoint);
        if ((distanceRight <= 20.0f or distanceLeft <= 20.0f or distanceDown <= 20.0f or distanceUp <= 20.0f) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::cout << "Collecting " << trees[i].type << std::endl;

            bool slotFound = false;
            for (auto &slot : inventory)
            {
                if (!slot.hasItem)
                {
                    // Place the tree in the empty slot
                    slot.hasItem = true;
                    slot.Itemtype = trees[i].type;
                    slot.color = trees[i].color;
                    slotFound = true;
                    break; // Exit the loop once the item is placed
                }
            }

            if (!slotFound)
            {
                std::cout << "Inventory is full!" << std::endl;
            }
            else
            {
                trees.erase(trees.begin() + i);
                i--;
            }
        }
    }
    // Handle interaction and drawing for trees
    for (const Tree &tree : trees)
    {
        HandleInteraction(player, tree, RightPoint, "Chopping down", 20.0f);
        DrawEntity(tree);
    }
    float interactiontime = GetFrameTime();

    // Handle interaction and drawing for villagers
    for (const Villager &villager : villagers)
    {
        float distanceRight = CalculateDistance(villager.position, RightPoint);
        float distanceLeft = CalculateDistance(villager.position, LeftPoint);
        float distanceUp = CalculateDistance(villager.position, UpPoint);
        float distanceDown = CalculateDistance(villager.position, DownPoint);

        if ((distanceRight <= 10.0f or distanceLeft <= 10.0f or distanceDown <= 10.0f or distanceUp <= 10.0f) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (villager.canTrade)
            {
                std::cout << "Trading with " << villager.name << std::endl;
                if (villager.name == "Trader Joe")
                {
                    InteractedWithVillager = true;
                }
            }
            else
            {
                // InteractedWithVillager = false;
                std::cout << InteractedWithVillager << std::endl;
                std::cout << "Talking to " << villager.name << std::endl;
            }
        }
        if (InteractedWithVillager)
        {
            HandleVillagerInteraction(villager, 20.0f, InteractedWithVillager);
        }
        // std::cout << "Is it still true: " << InteractedWithVillager << std::endl;
        DrawVillager(villager);
    }
    // Draw the inventory
    DrawInventory(inventory);
}

// Function to draw all entities in their respective groups
void DrawEntities(const std::vector<Mineral> &minerals, const std::vector<Tree> &trees, const std::vector<Enemies> &enemies, const std::vector<Villager> &villagers)
{

    // Draw minerals
    for (const auto &mineral : minerals)
    {
        DrawEntity(mineral);
    }

    // Draw trees
    for (const auto &tree : trees)
    {
        DrawEntity(tree);
    }

    // Draw enemies
    for (const auto &enemy : enemies)
    {
        DrawEntity(enemy);
    }

    // Draw villagers
    for (const auto &villager : villagers)
    {
        DrawVillager(villager);
    }
}

/*
           _                 _
     /\   (_)               | |
    /  \   _   ___ _   _ ___| |_ ___ _ __ ___
   / /\ \ | | / __| | | / __| __/ _ \ '_ ` _ \
  / ____ \| | \__ \ |_| \__ \ ||  __/ | | | | |
 /_/    \_\_| |___/\__, |___/\__\___|_| |_| |_|
                    __/ |
                   |___/
*/
//            Initial position, speed, direction, direction change interval, and timer
AIEntity ai = {{400.0f, 400.0f}, 50.0f, {0.0f, 0.0f}, 2.0f, 0.0f};

void UpdateAI(AIEntity &ai)
{
    // Update the timer
    ai.timeSinceLastChange += GetFrameTime();

    // Change direction at regular intervals
    if (ai.timeSinceLastChange >= ai.directionChangeInterval)
    {
        float randomAngle = GetRandomValue(0, 360) * DEG2RAD;
        ai.direction = {cosf(randomAngle), sinf(randomAngle)};
        ai.timeSinceLastChange = 0.0f;
    }

    // Update position
    ai.position.x += ai.direction.x * ai.speed * GetFrameTime();
    ai.position.y += ai.direction.y * ai.speed * GetFrameTime();

    // Keep AI within screen bounds
    if (ai.position.x < 0)
        ai.position.x = 0;
    if (ai.position.x > GetScreenWidth())
        ai.position.x = GetScreenWidth();
    if (ai.position.y < 0)
        ai.position.y = 0;
    if (ai.position.y > GetScreenHeight())
        ai.position.y = GetScreenHeight();
}

void DrawAI(const AIEntity &ai)
{
    DrawCircleV(ai.position, 10.0f, BLUE); // Draw AI as a blue circle
}

/*
 __          __                             _____           _
 \ \        / /                            / ____|         | |
  \ \  /\  / /__  __ _ _ __   ___  _ __   | (___  _   _ ___| |_ ___ _ __ ___
   \ \/  \/ / _ \/ _` | '_ \ / _ \| '_ \   \___ \| | | / __| __/ _ \ '_ ` _ \
    \  /\  /  __/ (_| | |_) | (_) | | | |  ____) | |_| \__ \ ||  __/ | | | | |
     \/  \/ \___|\__,_| .__/ \___/|_| |_| |_____/ \__, |___/\__\___|_| |_| |_|
                      | |                          __/ |
                      |_|                         |___/
*/

float Vector2Distance(Vector2 v1, Vector2 v2)
{
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

Vector2 Vector2Normalize(Vector2 v)
{
    float length = sqrt(v.x * v.x + v.y * v.y);
    if (length != 0)
    {
        return {v.x / length, v.y / length};
    }
    else
    {
        return {0, 0};
    }
}

void WeaponSystem(Player &player, const std::vector<CollideExample> &Wall, bool &collided, bool &explod)
{
    Vector2 mousePosition = GetMousePosition();
    static Vector2 EndPosition = {0.0f, 0.0f};
    static Vector2 BouncePosition = {0.0f, 0.0f};
    static Vector2 currentposition = {0.0f, 0.0f};
    float speed = 500.0f;

    DrawLineV(player.PlayerPosition, mousePosition, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        DrawCircleV(mousePosition, 5.0f, RED);
        currentposition = player.PlayerPosition;
        EndPosition = mousePosition;
        collided = false;
        explod = false;
    }

    Vector2 direction = Vector2Normalize({EndPosition.x - currentposition.x, EndPosition.y - currentposition.y});
    Vector2 velocity = {direction.x * speed, direction.y * speed};

    // Movement towards point of target if not collided
    if (!collided)
    {
        if (Vector2Distance(currentposition, EndPosition) > 1.0f)
        {
            currentposition.x += velocity.x * GetFrameTime();
            currentposition.y += velocity.y * GetFrameTime();
        }
    }

    for (const CollideExample &wall : Wall)
    {
        Rectangle wallrect = {wall.Position.x, wall.Position.y, wall.Width, wall.Height};

        if (CheckCollisionCircleRec(currentposition, 5.0f, wallrect))
        {
            if (wall.type == "exampletype")
            {
                explod = true;
            }
            // Determine wall normal based on collision side
            Vector2 wallNormal = {0, 0};
            float distanceToLeft = currentposition.x - wallrect.x;                       // Left side
            float distanceToRight = (wallrect.x + wallrect.width) - currentposition.x;   // Right side
            float distanceToTop = currentposition.y - wallrect.y;                        // Top side
            float distanceToBottom = (wallrect.y + wallrect.height) - currentposition.y; // Bottom side

            // Find the minimum distance to determine the wall side collided with
            float minDistance = fmin(fmin(distanceToLeft, distanceToRight),
                                     fmin(distanceToTop, distanceToBottom));

            // Assign wall normal based on which side was hit
            if (minDistance == distanceToLeft)
            {
                wallNormal = {-1, 0}; // Collided with the left side
                speed -= 100.0f;
            }
            else if (minDistance == distanceToRight)
            {
                wallNormal = {1, 0}; // Collided with the right side
                speed -= 100.0f;
            }
            else if (minDistance == distanceToTop)
            {
                wallNormal = {0, -1}; // Collided with the top side
                speed -= 100.0f;
            }
            else if (minDistance == distanceToBottom)
            {
                wallNormal = {0, 1}; // Collided with the bottom side
                speed -= 100.0f;
            }

            // Calculate the dot product
            float dotProduct = (velocity.x * wallNormal.x) + (velocity.y * wallNormal.y);

            // Reflect the velocity vector
            velocity.x = velocity.x - 2 * dotProduct * wallNormal.x;
            velocity.y = velocity.y - 2 * dotProduct * wallNormal.y;

            currentposition.x += wallNormal.x * 5.0f; // Move out based on the normal direction
            currentposition.y += wallNormal.y * 5.0f; // Move out based on the normal direction

            EndPosition = {velocity.x - (-velocity.x), velocity.y - (-velocity.y)};
        }
    }
    DrawCircleV(EndPosition, 3.0f, RED);
    DrawCircleV(currentposition, 10.0f, BLACK);
}

void Delete(const std::vector<Texture2D> &mineraltexture)
{
    for (const auto &texture : mineraltexture)
    {
        UnloadTexture(texture);
    }
    for (const villagerinventory Items : inventoryTrading){
        UnloadTexture(Items.texture);
    }
}