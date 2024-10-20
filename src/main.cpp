#include "raylib.h"
#include <iostream>

#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#pragma endregion

#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

// My own library
#include <GameHeader.h>
#include "MapGeneration.h"

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 900, "Survivor gameEngine");

#pragma region imgui
	rlImGuiSetup(true);

	// Themes
	imguiThemes::yellow();

	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.FontGlobalScale = 1;								  // font size

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}

#pragma endregion

	Texture2D PlayerSprite = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/rabbit.png");
	Texture2D gold = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Gold.png");
	Texture2D Silver = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Silver.png");
	std::vector<Texture2D> unloadingtextures = {gold, Silver};
	if (PlayerSprite.id == 0)
	{
		std::cerr << "Error: Could not load texture." << std::endl;
		return -1; // Exit if the texture failed to load
	}
	else
	{
		std::cerr << "Successfully loaded texture" << std::endl;
	}

	// Create player instance and set initial values
	Player player = {
		{100.0f, 100.0f}, // Initial Position
		{1.0f, 1.0f},
		200.0f,		 // Speed
		5,			 // Width
		10,			 // Height
		PlayerSprite // Texture
	};

	Enemies entity = {
		{300.0f, 300.0f}, // Initial position
		GREEN};

	// Create a mineral instance
	Mineral mineral = {
		{150.0f, 200.0f}, // Initial position
		YELLOW,			  // Color
		"Gold"			  // Type
	};

	// Create a tree instance
	Tree tree = {
		{400.0f, 350.0f}, // Initial position
		BROWN,			  // Color
		"Oak"			  // Type
	};

	// Create a villager instance
	Villager villager = {
		{100.0f, 100.0f}, // Initial position
		BLUE,			  // Color
		"Trader Joe",	  // Name
		true			  // Can trade
	};

	// If you want to use vectors for multiple instances:
	std::vector<Mineral> minerals = {
		{{100.0f, 150.0f}, YELLOW, "Gold", gold},
		{{200.0f, 250.0f}, DARKGRAY, "Silver", Silver}};

	std::vector<Tree> trees = {
		{{300.0f, 350.0f}, BROWN, "Oak"},
		{{400.0f, 450.0f}, BROWN, "Pine"}};

	std::vector<Enemies> enemies = {
		{{500.0f, 550.0f}, RED, "Goblin"},
		{{600.0f, 650.0f}, DARKGRAY, "Orc"}};

	std::vector<Villager> villagers = {
		// POSITION       COLOR    NAME       TRADEABLE
		{{300.0f, 350.0f}, BLUE, "Trader Joe", true},
		{{800.0f, 850.0f}, GREEN, "Merchant Mike", false}};

	// Variables for the inventory grid
	int slotsX = 5;			   // Number of slots horizontally
	int slotsY = 4;			   // Number of slots vertically
	float slotSize = 50.0f;	   // Width and height of each slot
	float slotPadding = 10.0f; // Padding between slots

	// Vector to hold the inventory slots
	std::vector<InventorySlot> inventories;

	// Initialize the inventory
	init(inventories, slotsX, slotsY, slotSize, slotPadding, GetScreenHeight());
	bool showinventory = false;

	// Texture loading
	// storeTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Crafting&Gathering/Gold.png");
	const char *tileset = "C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/Assets/Tileset.png";
	Texture2D tileSheet = LoadTexture("C:/Users/elias/Downloads/GameChallenge/raylibCmakeSetup-master/resources/newmaptile.png");
	Map *mapgenerationInformation;
	Vector2 startposition;
	// init(mapgenerationInformation, tileset);
	int map[10][10] = {
		{18, 18, 18, 18, 18, 18, 18, 18, 18, 19}, // Incremented top row
		{32, 33, 34, 34, 34, 34, 34, 34, 34, 35}, // Incremented second row
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35}, // Continue with incremented middle pieces
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{33, 34, 34, 34, 34, 34, 34, 34, 34, 35},
		{49, 50, 50, 50, 50, 50, 50, 50, 50, 51} // Incremented bottom row
	};

	int tilesize = 48;
	float scalefactor = 3.0f;
	//some comments
	bool InteractedVillager = false;
	std::cout << "hello beginning" << std::endl;
	bool examplebool = false;
	Rectangle wall = {350, 500, 200, 200};
	// Shatter animation
	// Rectangle exampleofrectanimation = {200, 200, 400, 200};
	Rectangles exampleofrect = {200, 200, 400, 200, "exampleType"};
	// Initialize CollideExample with the wall rectangle
	CollideExample wallExample = {{wall.x, wall.y}, (int)wall.width, (int)wall.height, RED, "null"};
	CollideExample explosionwall = {{exampleofrect.x, exampleofrect.y}, (int)exampleofrect.width, (int)exampleofrect.height, BLUE, "exampletype"};
	std::vector<CollideExample> wallrect = {wallExample, explosionwall};

	bool equiped = false;
	bool exploded = false;
	bool collided = false;

	// Here is for play around code
	Vector2 initialPosition = {500, 500};
	Rectangle dragrectangle = {initialPosition.x, initialPosition.y, 100, 100};

	// MAIN LOOP
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();
		bool ishovering = isMouseOverRectangle(dragrectangle);
		float deltaTime = GetFrameTime();
		BeginDrawing();
		ClearBackground(BLACK);
		Generate(mapgenerationInformation, map, tilesize, scalefactor, tileSheet);
		DrawFPS(10, 10);
		UpdateAI(ai);
		DrawAI(ai);
		PlayerCreation(player, enemies, minerals, trees, villagers, inventories, InteractedVillager);
		DrawEntities(minerals, trees, enemies, villagers);
		DrawRectangleRec(wall, RED);
		if (IsKeyPressed(KEY_E))
		{
			equiped = true;
		}
		else if (IsKeyPressed(KEY_Q))
		{
			equiped = false;
		}
		if (equiped)
			WeaponSystem(player, wallrect, collided, exploded);

		if (!exploded)
		{
			InitShatterPieces((Rectangle){exampleofrect.x, exampleofrect.y, exampleofrect.width, exampleofrect.height});
		}
		if (IsKeyPressed(KEY_R))
		{
			exploded = false;
		}
		UpdateShatterPieces(deltaTime);
        
		//Test for seeing if the hovering works
		if (ishovering && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			initialPosition = mousePos;
			dragrectangle.x = initialPosition.x - dragrectangle.width / 2;	// Update x position of the rectangle
			dragrectangle.y = initialPosition.y - dragrectangle.height / 2; // Update y position of the rectangle
		}
		DrawRectangle(dragrectangle.x, dragrectangle.y, dragrectangle.width, dragrectangle.height, BLACK);
		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (!exploded)
		{
			DrawRectangleRec((Rectangle){exampleofrect.x, exampleofrect.y, exampleofrect.width, exampleofrect.height}, BLUE); // Draw the original rectangle
		}
		else
		{
			DrawShatterPieces(); // Draw the shattered pieces
		}

		// Imgui Stuff
#pragma region imgui
		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

		ImGui::Begin("Debugger");
		ImGui::Text("Press R to reset the shatter animation");
		ImGui::End();
#pragma region imgui
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#pragma endregion

		EndDrawing();
	}
	deleteTexture(tileSheet);
	// Deletion
	Delete(unloadingtextures);

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

	CloseWindow();
	return 0;
}